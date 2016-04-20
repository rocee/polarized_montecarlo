#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Vector.h"

#ifndef __photons__h
#define __photons__h 1

#define ALIVE 1
#define DEAD 0

// TODO: 
// make the effects of diattenuation optional (turn on or off)
// decide whether or not to do the sphere or cylinder at every scattering encounter
//
// Get spherical model working using ramella code or other lit
// test decision making by choosing two different diameters of spherical scatterers
//
// Make modular? more general structure, additional components for later montecarlo
//
// swappable sizes, polarization or not, birefringence or not, etc.
//
// new phase functions or not
//
// Validation using newest Ramella paper. compare to ramella output for validation.



/* Properties of the light defined in main.cpp */
extern double *s11, *s12, *s33, *s43;
extern double I_R, Q_R, U_R, V_R, I_T, Q_T, U_T, V_T;
extern double mu_a, mu_s, slabdepth, albedo, g;
extern int nphotons, nangles;

/* Properties of our model (taken from Ramella et. al., 2005) */
#define WEIGHT_CUTOFF 0.01 // From Ramella et. al.
const double survival_multiplier = 10;
const double survival_chance = 1 / survival_multiplier;

/* Some random number generator that produces a uniform 
 * distribution of numbers from (0,1].
 */
double rand_num() {
   double r = 0;
   do {
     r = ((double) rand() / RAND_MAX); 
   } while (r == 0);
   return r;
}

/* Track polarization state of photon via Stokes vector */
struct stokes_v {
public:
    double I;
    double Q;
    double U;
    double V;
    stokes_v(void) { I = 0; Q = 0; U = 0; V = 0; }
    stokes_v(double I, double Q, double U, double V) : I(I), Q(Q), U(U), V(V) { }; 
	stokes_v(const stokes_v& that) {
		this->I = that.I;
		this->Q = that.Q;
		this->U = that.U;
		this->V = that.V;
	}

    void scalar_mult(double x) { I = x*I; Q = x*Q; U = x*U; V = x*V;}    

	/*
	 * Rotation of Stokes vector by angle phi
	 * about the axis of propogation
	 */
	void rotate_stokes(double phi) {                                     
		double cos_2phi = cos(2*phi);                                    
		double sin_2phi = sin(2*phi);                                	
		double old_q = this->Q;
		double old_u = this->U;
		this->Q = old_q*cos_2phi + old_u*sin_2phi;
		this->U = -old_q*sin_2phi + old_u*cos_2phi;
	}

    void normalize() {
        if (I <= 1e-6) {printf("Small intensity problem!\n"); return; }
        Q = Q/I;
        U = U/I;
        V = V/I;
        I = 1.0;
    }
};

struct quaternion {
public:
	double q0;
	Vector gam;
	quaternion(void) {
		q0 = 0;
	}
	quaternion(double q0, double i, double j, double k) : q0(q0) {
		Vector a(i,j,k);
		gam = a;
	}
	// Constructor for rotational quaternion
	// Represents a rotation about axis by angle.
	quaternion(double angle, Vector axis) {
		double sin_ang = sin(angle/2);
		double cos_ang = cos(angle/2);
		q0 = cos_ang;
		Vector a(axis.i*sin_ang, axis.j*sin_ang, axis.k*sin_ang);
		gam = a;
	}

	quaternion inv(void) {
		quaternion result = quaternion(q0,-gam.i, -gam.j, -gam.k);
		return result;
	}

	static quaternion hprod(quaternion &b, quaternion &a) {
		double ret_q0 = b.q0*a.q0 - (b.gam.i*a.gam.i) - (b.gam.j*a.gam.j) - (b.gam.k*a.gam.k);
		double ret_g1 = b.q0*a.gam.i + (b.gam.i*a.q0) + (b.gam.j*a.gam.k) - (b.gam.k*a.gam.j);
		double ret_g2 = b.q0*a.gam.j - (b.gam.i*a.gam.k) + (b.gam.j*a.q0) + (b.gam.k*a.gam.i);
		double ret_g3 = b.q0*a.gam.k + (b.gam.i*a.gam.j) - (b.gam.j*a.gam.i) + (b.gam.k*a.q0);
		quaternion result = quaternion(ret_q0, ret_g1, ret_g2, ret_g3);
		return result;
	}

	Vector vector_prod(Vector &a) {
		quaternion v = quaternion(0,a.i, a.j, a.k);
		quaternion intermediate = hprod(*this, v);
		quaternion q_inv = this->inv();
		quaternion result = hprod(intermediate, q_inv);
		return result.gam;

	}

};

struct photon {
private:
    double weight;
    bool state; // Alive or dead
public:
	double alpha; // scattering angles
	double beta; // scattering angles
    Vector V; // Orientation vector (Stokes Y axis)
    Vector U; // Direction of Propogation (Stokes Z axis) 
    double x; // Lateral x position (cm)
    double y; // Lateral y position (cm)
    double z;
    stokes_v S;
    // Default constructor.
    photon(void) {
        weight = 1;
        // Start at center and top of slab
        x = 0.0; 
        y = 0.0;
        z = 0.0;

        S.I = 0.0;
        S.Q = 0.0;
        S.U = 0.0;
        S.V = 0.0; 

        U.i = 0.0;
        U.j = 0.0;
        U.k = 1.0;

        V.i = 0.0;
        V.j = 1.0;
        V.k = 0.0;

        state = ALIVE;
	    alpha = 0; // scattering angles
	    beta = 0; // scattering angles
    } 

	void setStokes(double i, double q, double u, double v) {
		S.I = i;
		S.Q = q;
		S.U = u;
		S.V = v;
	}

    bool alive() {
        return state; 
    }


	void rotate_about_vector(Vector& trajectory, Vector& axis, double angle) {
		int columns = 3;
		int rows = 3;	
		double** r_euler = new double* [columns];
		for (int i = 0; i < columns; i += 1) {
			r_euler[i] = new double [rows];
		}

		// Define a rotational matrix R_{euler}
		r_euler[0][0] = axis.i*axis.i*(1- cos(angle));
		r_euler[1][0] = axis.j*axis.i*(1- cos(angle)) - axis.k*sin(angle);	
		r_euler[2][0] = axis.k*axis.i*(1- cos(angle)) + axis.j*sin(angle);
		r_euler[0][1] = axis.i*axis.j*(1- cos(angle)) + axis.k*sin(angle);	
		r_euler[1][1] = axis.j*axis.j*(1- cos(angle)) + cos(angle);	
		r_euler[2][1] = axis.j*axis.k*(1- cos(angle)) - axis.i*sin(angle);	
		r_euler[0][2] = axis.i*axis.k*(1- cos(angle)) - axis.j*sin(angle);	
		r_euler[1][2] = axis.j*axis.k*(1- cos(angle)) + axis.i*sin(angle);	
		r_euler[2][2] = axis.k*axis.k*(1- cos(angle)) + cos(angle);	

		// Make vectors into arrays for easier multiplication
		double* v = new double [3];	
		v[0] = trajectory.i;	
		v[1] = trajectory.j;	
		v[2] = trajectory.k;	

		double* new_v = new double [3];	
		new_v[0] = 0;	
		new_v[1] = 0;	
		new_v[2] = 0;

		// Multiply trajectory by rotational matrix.
		for (int i = 0; i < 3; i += 1) {
			for (int j = 0; j < 3; j += 1) {
				new_v[i] += v[j] * r_euler[j][i];
			}			
		}

		// Update trajectory vector
		trajectory.i = new_v[0];
		trajectory.j = new_v[1];
		trajectory.k = new_v[2];	

		delete [] r_euler;
		delete [] v;
	}
	
	/* 
     * move(void) -
     * Adjust position of photon in space based upon
     * orientation and properties of the medium.
     */
    void move(void) {
		double rnum = 0;
		do { rnum = rand_num(); } while (rnum == 0);
        double deltaS = -log(rnum) / (mu_a+mu_s);
        x += U.i * deltaS;
        y += U.j * deltaS;
        z += U.k * deltaS;
    }

    /* 
     * drop(void) -
     * Drop weight from the photon and determine if it is dead
     * or out of bounds. Adjust weights accordingly. If photon
	 * is dead, re-orient it to detector and record stokes vector.
     */
    void drop(void) {
        weight *= albedo; 
        // Check weight
        if (weight < WEIGHT_CUTOFF) {  
            // Russian Roulette for photon absorption
            double rand = rand_num();
            if (rand <= survival_chance) {
                weight = 0;  
                state = DEAD;
                //printf("Dead by absorption\n");
            } else {    
                weight *= survival_multiplier; 
            } 
        }
		
        // Check boundaries
		if (this->z <= 0) { // Photon is reflected
			// Rotate axes back to reference frame
			Vector W = Vector::cross_prod(V,U);
			if ((V.k <= 1e-8) && (W.k <= 1e-8)) {  }
			else { 
			double epsilon = atan2(V.k , -W.k); 
			S.rotate_stokes(epsilon);
			double phi = atan2(U.j, U.i);
			S.rotate_stokes(phi);
			}
			I_R += S.I;
			Q_R += S.Q;
			U_R += S.U;
			V_R += S.V;
			state = DEAD;
		}
		else if (this->z >= slabdepth) { // Photon is transmitted
			// Rotate axes back to reference frame
			Vector W = Vector::cross_prod(V,U);
			if ((V.k <= 1e-8) && (W.k <= 1e-8)) { }
			else {
			double epsilon = atan2(V.k , -W.k); 
			S.rotate_stokes(epsilon);
			double phi = -1*atan2(U.j, U.i);
			S.rotate_stokes(phi);
			}
			I_T += S.I;
			Q_T += S.Q;
			U_T += S.U;
			V_T += S.V;
			state = DEAD;

		}
    }

    /*
     * rejection() - choose alpha and beta angles for scattering
     */
	void rejection(void) {
        // Placeholder locals for choosing rejection angle alpha
        double theta, phi, Io, Iith, rand_no;
		int i = 0;
        do {
			i++;
            //printf("S.I: %5.5f  S.Q: %5.5f  S.U: %5.5f  S.V: %5.5f\n",S.I,S.Q,S.U,S.V);
            theta = acos(2*rand_num() - 1); // TODO: why do we need to do 2*rand - 1? isn't this redundant?
            phi = 2*M_PI*rand_num();
            Io = s11[0]*S.I + s12[0]*(S.Q*cos(2*phi) + S.U*sin(2*phi));
            int i = floor(theta / M_PI * nangles);
            Iith = s11[i]*S.I + s12[i]*(S.Q*cos(2*phi) + S.U*sin(2*phi));
            rand_no = rand_num();
            //printf("Rand_no: %5.5f\nIo: %5.5f\nIith: %5.5f\n",rand_no,Io, Iith);
        } while ((rand_no*Io) >= Iith);
		//printf("Rejection Iterations: %d\n",i);
		alpha = theta;
		beta = phi;
		return;
	}

	void quaternion_scatter(void) {
		/* Step 1: Rotate V about U by beta */
		quaternion rot_U_b(beta, U);
		Vector newV = rot_U_b.vector_prod(V);
		V = newV;

		/* Step 2: Rotate U about V by alpha */
		quaternion rot_V_a(alpha,V);
		Vector newU = rot_V_a.vector_prod(U);
		U = newU;

        /* Step 3: Rotate Stokes vector by beta */
        // Placeholders for step 4
        S.rotate_stokes(beta);
        double si = S.I;
        double sq = S.Q;
        double su = S.U;
        double sv = S.V;

        /* Step 4: Multiply Stokes by scattering matrix */
        int ithedeg = floor(alpha/M_PI*nangles);
		S.I= s11[ithedeg]*si+s12[ithedeg]*sq;
		S.Q= s12[ithedeg]*si+s11[ithedeg]*sq;
		S.U= s33[ithedeg]*su+s43[ithedeg]*sv;
		S.V= -s43[ithedeg]*su+s33[ithedeg]*sv;
		S.normalize();
	}
};

#endif
