typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;
typedef float4 torus;
typedef float4 cone;


vector
Bounce( vector in, vector n )
{
	vector out = in - n*(vector)( 2.*dot(in.xyz, n.xyz) );
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector v, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( v, n );
}

/*vector
BounceTorus(point p, vector v, torus t)
{
	vector n;
	n.xyz = fast_normalize(p.xyz - t.xyz);
	n.w = 0.;
	return Bounce(v, n);
}

vector
BounceCone(point p, vector v, cone c)
{
	vector n;
	n.xyz = fast_normalize(p.xyz - c.xyz);
	n.w = 0.;
	return Bounce(v, n);
}

bool
IsInsideCone(point p, cone c)
{
	float r = fast_length(p.xyz - c.xyz);
	return  (r < c.w);
}

bool
IsInsideTorus( point p, torus t )
{
	float r = fast_length( p.xyz - t.xyz );
	return  ( r < t.w );
}(*/

bool
IsInsideSphere(point p, sphere s)
{
	float r = fast_length(p.xyz - s.xyz);
	return  (r < s.w);
}


kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;
	//const sphere Sphere1 = (sphere)( -100., -800., 0., 600);
	//const torus Torus1 = (torus)(-100., -800., 0., 100);
	//const cone Cone1 = (cone)(-100., -1400., 10., 400.);
	const sphere Sphere2 = (sphere)(-100., -800., 0., 200.);
	const sphere Sphere3 = (sphere)(-100.,-1200., 0., 400.);
	const sphere Sphere4 = (sphere)(900., -800., 0., 200.);
	const sphere Sphere5 = (sphere)(900.,-1200., 0., 400.);
	const sphere Sphere6 = (sphere)(-1000., -800., 0., 200.);
	const sphere Sphere7 = (sphere)(-1000.,-1200., 0., 400.);
	//const sphere Sphere8 = (sphere)(-100.,-1600., 0., 200.);
	//const sphere Sphere9 = (sphere)(900., -1600., 0., 200.);
	//const sphere Sphere10 = (sphere)(-1000.,-1600., 0., 200.);

	int gid = get_global_id( 0 ); (-100., -800., 0., 200.);
	
	// extract the position and velocity for this particle:
	point  p = dPobj[gid];
	vector v = dVel[gid];
	color  c = dCobj[gid];

	// remember that you also need to extract this particle's color
	// and change it in some way that is obviously correct

	// advance the particle:

	point  pp = p + v*DT + G*(point)( .5*DT*DT );
	vector vp = v + G*DT;
	pp.w = 1.;
	vp.w = 0.;
	

	/*c += (float4)(.01, 0.000, 0.000, 0);
	if(c.x >= 1){
		c.x = 0;
	}*/	
	
	// test against the first sphere here:

	/*if( IsInsideSphere( pp, Sphere1 ) )
	{
		vp = BounceSphere( p, v, Sphere1 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );

		c = (float4)(.8f, 0., .8f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
	}*/

	// test against the second sphere here:

	if(IsInsideSphere(pp, Sphere2))
	{
		//vp = BounceSphere(p, v, Sphere2);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.8f, 0., .8f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 0;
		}
		else if(c.z >= 1){
			c.z = 0;
		}

	}
	else if (IsInsideSphere(pp, Sphere3))
	{
		vp = BounceSphere(p, v, Sphere3);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.1f, .6f, .8f, 0);
		if(c.x >= 1){
			c.x = 0.1;
		}
		else if(c.y >= 1){
			c.y = 0.6;
		}
		else if(c.z >= 1){
			c.z = 0.8;
		}
	}
	else if(IsInsideSphere(pp, Sphere4))
	{
		//vp = BounceSphere(p, v, Sphere4);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(0.f, 1.f, 0.f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 1.;
		}
		else if(c.z >= 1){
			c.z = 0;
		}

	}
	else if (IsInsideSphere(pp, Sphere5))
	{
		vp = BounceSphere(p, v, Sphere5);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(1.f, 1.f, 1.f, 0);
		if(c.x >= 1){
			c.x = 1;
		}
		else if(c.y >= 1){
			c.y = 1;
		}
		else if(c.z >= 1){
			c.z = 1;
		}
	}
	else if(IsInsideSphere(pp, Sphere6))
	{
		//vp = BounceSphere(p, v, Sphere6);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.86f, .57f, 0.f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 0;
		}
		else if(c.z >= 1){
			c.z = 0;
		}

	}
	else if (IsInsideSphere(pp, Sphere7))
	{
		vp = BounceSphere(p, v, Sphere7);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(0.f, 0.f, .5f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 0;
		}
		else if(c.z >= 1){
			c.z = .5;
		}
	}
	/*else if (IsInsideSphere(pp, Sphere8)||IsInsideSphere(pp, Sphere9)||IsInsideSphere(pp, Sphere10))
	{
		//vp = BounceSphere(p, v, Sphere7);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		//c = (float4)(0.f, 0.f, .5f, 0);
		if(IsInsideSphere(pp, Sphere8)){
			c.x = 0.1;
			c.y = 0.6;
			c.z = 0.8;
		}
		else if(IsInsideSphere(pp, Sphere9)){
			c.x = 1;
			c.y = 1;
			c.z = 1;
		}
		else if(IsInsideSphere(pp, Sphere10)){
			c.x = 0;
			c.y = 0;
			c.z = .5;
		}
	}*/

	c += (float4)(.003, 0.003, 0.003, 0);
	/*else if (IsInsideTorus(pp, Torus1))
	{
		//vp = BounceTorus(p, v, Torus1);
		//pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.2f, 3.f, .8f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 0;
		}
		else if(c.z >= 1){
			c.z = 0;
		}
	}
	else if (IsInsideCone(pp, Cone1))
	{
		vp = BounceCone(p, v, Cone1);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.8f, .6f, .2f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 0;
		}
		else if(c.z >= 1){
			c.z = 0;
		}
	}*/


	dPobj[gid] = pp;
	dVel[gid]  = vp;
	dCobj[gid] = c;
}
