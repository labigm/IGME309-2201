/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/07
Based on Joshua McMahan's Rigid Body implementation
----------------------------------------------*/
#ifndef __SOLVER_H_
#define __SOLVER_H_

#include "Simplex\Physics\RigidBody.h"

namespace Simplex
{

class SimplexDLL Solver
{
public:
	Solver() //Default Constructor
	{
	};

	Solver(vector3 PosArg, quaternion OriArg, float MasArg) //Constructor
	{
		position = PosArg;
		orientation = OriArg;
		mass = MasArg;
		inverseMass = MasArg * -1;
	};
	Solver(Solver const& other) //Copy Constructor
	{
		position = other.position;
		orientation = other.orientation;
		mass = other.mass;
		inverseMass = other.mass * -1;
	};
	Solver& operator=(Solver const& other) //Copy operator
	{
		position = other.position;
		orientation = other.orientation;
		mass = other.mass;
		inverseMass = other.mass * -1;
		return *this;
	};
	~Solver() //destructor
	{

	};

	float x; //position of the object in 2d space
	float v; //velocity of the object in 2d space 

	vector3 position; //Position of the object in 3d space - location in 3d space
	vector3 momentum; //momentum of the object in 3d space - forces still acting on an object prior to full stop
	quaternion orientation; //orientation of the object in 3d space - direction object is "facing", used to simulate tiping the object
	vector3 angularMomentum; //angular momentum of the object in 3d space - used to affect the orientation from forces that would tip the object

	vector3 velocity; //velocity of the object in 3d space - moves the object around in 3d space, changing position
	quaternion spin; //spin refers to the orientation being affected by angular velocity - used to turn and tip the object
	vector3 angularVelocity; //moves the object around its local axis so that is can change its orientation

	float mass; //how heavy a given object is - used in calculating momentum and inertia
	float inverseMass; //inverse of the mass
	float inertia; //
	float inverseInertia; //inverse of the inertia

	float MAX_VEL = 0.002f;

	void recalculate() //Calculates all, and I mean ALL of the important bits that are used for the dooblydoo. If you don't call this, you better be hard coding this shit somewhere!
	{
		velocity = momentum*inverseMass; //calculate velocity. Commented temp.

		angularVelocity = angularMomentum*inverseInertia; //calculate angular velocity

														  //I need to normalize the orientation. this is the equation, but i need to just do something like orientation.normalize but that is not here. Working on a work around.
														  //orientation = (orientation.x + orientation.y + orientation.z + orientation.w)/(sqrt((orientation.x*orientation.x) + (orientation.y*orientation.y) + (orientation.z*orientation.z) + (orientation.w*orientation.w)));

		quaternion q(0, angularVelocity.x, angularVelocity.y, angularVelocity.z); //temp quat too store angular velocity as to affect spin

		spin = 0.5f * q * orientation; //calculate spin
	};

	void slowDown() //Used to slow down the object if no key is pressed. Should run every frame that the object is not at a standstill
	{
		velocity.x *= 0.96f;
		velocity.y *= 0.96f;
		velocity.z *= 0.96f;
	};

#pragma region CheckOrien
	void checkOrienX() //Checks Orientation for the next method, continuteToRotate
	{
		if (orientation.x == 0 || orientation.x == 90 || orientation.x == 180 || orientation.x == 270 || orientation.x >= 360)
		{
			if (orientation.x >= 360)
			{
				orientation.x = 0;
			}
			return;
		}

		if (orientation.x > 0 && orientation.x < 45)
		{
			orientation.x -= 1;
			return;
		}
		if (orientation.x > 45 && orientation.x < 90)
		{
			orientation.x += 1;
			return;
		}
		if (orientation.x > 90 && orientation.x < 135)
		{
			orientation.x -= 1;
			return;
		}
		if (orientation.x > 135 && orientation.x < 180)
		{
			orientation.x += 1;
			return;
		}
		if (orientation.x > 180 && orientation.x < 225)
		{
			orientation.x -= 1;
			return;
		}
		if (orientation.x > 225 && orientation.x < 270)
		{
			orientation.x += 1;
			return;
		}
		if (orientation.x > 270 && orientation.x < 315)
		{
			orientation.x -= 1;
			return;
		}
		if (orientation.x > 315 && orientation.x < 360)
		{
			orientation.x += 1;
			return;
		}
	};
	void checkOrienY()
	{
		//Items for y
		if (orientation.y == 0 || orientation.y == 90 || orientation.y == 180 || orientation.y == 270 || orientation.y >= 360)
		{
			if (orientation.y >= 360)
			{
				orientation.y = 0;
			}
			return;
		}

		if (orientation.y > 0 && orientation.y < 45)
		{
			orientation.y -= 1;
			return;
		}
		if (orientation.y > 45 && orientation.y < 90)
		{
			orientation.y += 1;
			return;
		}
		if (orientation.y > 90 && orientation.y < 135)
		{
			orientation.y -= 1;
			return;
		}
		if (orientation.y > 135 && orientation.y < 180)
		{
			orientation.y += 1;
			return;
		}
		if (orientation.y > 180 && orientation.y < 225)
		{
			orientation.y -= 1;
			return;
		}
		if (orientation.y > 225 && orientation.y < 270)
		{
			orientation.y += 1;
			return;
		}
		if (orientation.y > 270 && orientation.y < 315)
		{
			orientation.y -= 1;
			return;
		}
		if (orientation.y > 315 && orientation.y < 360)
		{
			orientation.y += 1;
			return;
		}
	};
	void checkOrienZ()
	{
		//Items for z
		if (orientation.z == 0 || orientation.z == 90 || orientation.z == 180 || orientation.z == 270 || orientation.z >= 360)
		{
			if (orientation.z >= 360)
			{
				orientation.z = 0;
			}
			return;
		}

		if (orientation.z > 0 && orientation.z < 45)
		{
			orientation.z -= 1;
			return;
		}
		if (orientation.z > 45 && orientation.z < 90)
		{
			orientation.z += 1;
			return;
		}
		if (orientation.z > 90 && orientation.z < 135)
		{
			orientation.z -= 1;
			return;
		}
		if (orientation.z > 135 && orientation.z < 180)
		{
			orientation.z += 1;
			return;
		}
		if (orientation.z > 180 && orientation.z < 225)
		{
			orientation.z -= 1;
			return;
		}
		if (orientation.z > 225 && orientation.z < 270)
		{
			orientation.z += 1;
			return;
		}
		if (orientation.z > 270 && orientation.z < 315)
		{
			orientation.z -= 1;
			return;
		}
		if (orientation.z > 315 && orientation.z < 360)
		{
			orientation.z += 1;
			return;
		}
	};
#pragma endregion

	void continueToRotate() //Used to see if the object has fully rotated to a certain 90* point. Only 90 since we are workming with box collisions
	{
		checkOrienX();
		checkOrienY();
		checkOrienZ();
	};

	//Stores values that are derived from base ones
	struct Derivative
	{
		float dx; //dx/dt = vel in 2d
		float dv; //dv/dt = spd in 2d

		vector3 velocity; //vel in 3d
		vector3 force; //3d forces

		quaternion spin; //orientation changing force
		vector3 torque; //acceleration force for spining
	};

	//object acceleration
	float acceleration(const State &state, float t)
	{
		const float k = 10;
		const float b = 1;
		return -k * x - b*v;
	};

	//calculates the derivative of a given object's state
	Derivative Evaluate(const Solver &initial, float t, float dt, const Derivative &d)
	{
		//calculating a temp state from the object's passed in state
		State state;
		x = initial.x + d.dx*dt;
		v = initial.v + d.dv*dt;

		Derivative output; //Temp to store the output

		output.dx = v; //get dx
		output.dv = acceleration(state, t + dt); //get dv

		return output;
	};

	//Integrates the values of an object state
	void Integrate(Solver &state, float t, float dt)
	{
		Derivative a, b, c, d; //temp values

								//Evaluate all of the derivatives
		a = Evaluate(state, t, 0.0f, Derivative());
		b = Evaluate(state, t, dt*0.5f, a);
		c = Evaluate(state, t, dt*0.5f, b);
		d = Evaluate(state, t, dt, c);

		//calk the Dx and Dt
		float dXdT = 1.0f / 6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
		float dVdT = 1.0f / 6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

		//Return the values to the state
		x = x + dXdT * dt;
		v = v + dVdT * dt;
	};

	//Returns the calculated torque of an objects state
	vector3 Torque(const State &state, float t)
	{
		return vector3(1, 0, 0) - angularVelocity * 0.1f;
	};
};

EXPIMP_TEMPLATE template class SimplexDLL std::vector<Solver>;
EXPIMP_TEMPLATE template class SimplexDLL std::vector<Solver*>;

}

#endif //__SOLVER_H_