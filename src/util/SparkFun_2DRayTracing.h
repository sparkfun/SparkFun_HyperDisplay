/*

SparkFun_2DRayTracing.h

Purpose:

Author: Owen Lyke

*/


#ifndef SF_2DRAYTRACING_H
#define SF_2DRAYTRACING_H

#include "Arduino.h"

// This section allows you to easily reconfigure the types used in this header file
typedef uint16_t 	sf2drt_max_qty_t;
typedef double 		sf2drt_extent_t;		
typedef double		sf2drt_diff_t;
typedef double		sf2drt_rotation_t;


struct sf2drt_coordinate_t{
	sf2drt_extent_t x;
	sf2drt_extent_t y;
};

struct sf2drt_bounds_t{
	sf2drt_extent_t minx;
	sf2drt_extent_t miny;
	sf2drt_extent_t maxx;
	sf2drt_extent_t maxy;
};

typedef enum{
	sf2drt_orientation_positive = 0x00,
	sf2drt_orientation_colinear,
	sf2drt_orientation_negative
}sf2drt_orientation_t;


// Triplets are ordered sets of three coordinates
// Because the order matters they have an orientation
class sf2drt_triplet{
protected:
	public: // temporary
	sf2drt_coordinate_t _p1;
	sf2drt_coordinate_t _p2;
	sf2drt_coordinate_t _p3;

public:
	sf2drt_orientation_t orientation( void )
	{
		// The cross product way
		double cross = ((double)_p2.y - (double)_p1.y) * ((double)_p3.x - (double)_p2.x) - ((double)_p2.x - (double)_p1.x) * ((double)_p3.y - (double)_p2.y);

		if(cross == 0){ return sf2drt_orientation_colinear; }
		if(cross > 0){ return sf2drt_orientation_negative; }
		else{ return sf2drt_orientation_positive; }
	}
	bool p3contained( void )
	{
		if((_p3.x <= max(_p1.x, _p2.x)) && (_p3.x >= min(_p1.x, _p2.x)) && (_p3.y <= max(_p1.y, _p2.y)) && (_p3.y >= min(_p1.y, _p2.y))){ return true; }
		return false;
	}

	sf2drt_triplet(sf2drt_coordinate_t p1, sf2drt_coordinate_t p2, sf2drt_coordinate_t p3)
	{
		_p1 = p1;
		_p2 = p2;
		_p3 = p3;
	}
};

// Lines are composed of two coordinates
class sf2drt_line{
protected:
	public: // temporary
	sf2drt_coordinate_t _p1;
	sf2drt_coordinate_t _p2;

public:
	bool intersects(sf2drt_line other)
	{
		sf2drt_triplet t11(_p1, _p2, other._p1);
		sf2drt_triplet t12(_p1, _p2, other._p2);
		sf2drt_triplet t21(other._p1, other._p2, _p1);
		sf2drt_triplet t22(other._p1, other._p2, _p2);
		sf2drt_orientation_t o11 = t11.orientation();
		sf2drt_orientation_t o12 = t12.orientation();
		sf2drt_orientation_t o21 = t21.orientation();
		sf2drt_orientation_t o22 = t22.orientation();


		// printOrientationInfo(t11, t12, t21, t22); 										// temporary


		if((o11 != o12) && (o21 != o22)){ return true; }								// General case
		if((o11 == sf2drt_orientation_colinear) && t11.p3contained()){ return true; } 	// p1, p2, and other p1 are colinear, and other p1 is within bounding box formed by p1 and p2
		if((o21 == sf2drt_orientation_colinear) && t21.p3contained()){ return true; } 	// p1, p2, and other p2 are colinear and other p2 is within...
		if((o12 == sf2drt_orientation_colinear) && t12.p3contained()){ return true; } 
		if((o22 == sf2drt_orientation_colinear) && t22.p3contained()){ return true; } 
		return false;
	}

	void setP1(sf2drt_coordinate_t p1){ _p1 = p1; }
	void setP2(sf2drt_coordinate_t p2){ _p2 = p2; }

	sf2drt_line(sf2drt_coordinate_t p1, sf2drt_coordinate_t p2)
	{
		_p1 = p1;
		_p2 = p2;
	}
};





// A general base class makes provisions for an arbitrary area that either contains or does not contain a particular point
class sf2drt_area{
protected:
	sf2drt_area()
	{

	}
	virtual bool contains(sf2drt_coordinate_t p) = 0;
};

// A polygon is an area defined by a finite number of straight lines that close
class sf2drt_polygon : public sf2drt_area{

public:
	sf2drt_max_qty_t _num_sides; 	// Equivalent to num_points for all CLOSED shapes - we assume polygon areas are closed from last point to first
	sf2drt_coordinate_t* _p_points; 	// Number  of elements in this array should be num_sides
	sf2drt_rotation_t	_rotation;


	sf2drt_bounds_t getBounds( void )
	{
		sf2drt_bounds_t bounds;

		bounds.maxx = (*(_p_points)).x;
		bounds.minx = (*(_p_points)).x;
		bounds.maxy = (*(_p_points)).y;
		bounds.miny = (*(_p_points)).y;

		for(sf2drt_max_qty_t indi = 1; indi < _num_sides; indi++)
		{
			bounds.maxx = max(bounds.maxx, (*(_p_points + indi)).x);
			bounds.minx = min(bounds.minx, (*(_p_points + indi)).x);
			bounds.maxy = max(bounds.maxy, (*(_p_points + indi)).y);
			bounds.miny = min(bounds.miny, (*(_p_points + indi)).y);
		}
		return bounds;
	}


	bool contains(sf2drt_coordinate_t p)
	{
		// Find maximum extent of the polygon in x direction
		sf2drt_bounds_t bounds = getBounds();

		if((p.x < bounds.minx) || (p.x > bounds.maxx) || (p.y < bounds.miny) || (p.y > bounds.maxy)){ return false; }

		// Make a line from the point of interest to the right until the maximum size
		sf2drt_coordinate_t p2 = p;
		p2.x = max(bounds.maxx, p2.x);
		sf2drt_line bisector(p, p2);

		uint8_t inside = 0x00;
		sf2drt_max_qty_t indi = 0;
		for(indi = 0; indi < _num_sides - 1; indi++)
		{
			sf2drt_line segment((*(_p_points + indi)), (*(_p_points + indi + 1)));
			if(bisector.intersects(segment)){ inside ^= 0x01; }
		}
		sf2drt_line segment((*(_p_points + indi)), (*(_p_points)));
		if(bisector.intersects(segment)){ inside ^= 0x01; }

		if(inside & 0x01)
		{
			return true;
		}
		return false;
	}

	void displace(sf2drt_diff_t dx, sf2drt_diff_t dy)
	{
		for(sf2drt_max_qty_t indi = 0; indi < _num_sides; indi++)
		{
			(_p_points + indi)->x += dx;
			(_p_points + indi)->y += dy;
		}
	}


	void setPn(sf2drt_max_qty_t n, sf2drt_coordinate_t p)
	{
		if(n < _num_sides)
		{
			*(_p_points + n) = p;
		}
	}

	sf2drt_coordinate_t getPn(sf2drt_max_qty_t n)
	{
		if(n < _num_sides)
		{
			return *(_p_points + n);
		}
	}

	void rotate(sf2drt_rotation_t alpha, sf2drt_coordinate_t center)
	{
		// Positive or negative rotation by the parameter alpha about the coordinate center
		_rotation += alpha; // 

	}

	sf2drt_coordinate_t getCOM( void )
	{
		// Return the "center of mass" of the polygon (average location of all vertices, actually)
		sf2drt_coordinate_t retval = {0,0};

		double avgX = 0;
		double avgY = 0;

		for(sf2drt_max_qty_t indi = 0; indi < _num_sides; indi++)
		{
			avgX += (double)(_p_points + indi)->x;
			avgY += (double)(_p_points + indi)->y;
		}

		avgX /= (double)_num_sides;
		avgY /= (double)_num_sides;

		retval.x = (sf2drt_extent_t)round(avgX);
		retval.y = (sf2drt_extent_t)round(avgY);

		return retval;
	}

	sf2drt_polygon()
	{
		_p_points = NULL;
		_num_sides = 0;
		_rotation = 0;
	}

	sf2drt_polygon(sf2drt_max_qty_t num_sides, sf2drt_coordinate_t* p_points)
	{
		_p_points = p_points;
		_num_sides = num_sides;
		_rotation = 0;
	}
};

// You could add specific polygons, in case you thought it useful


class sf2drt_equilateral_center_tip : public sf2drt_polygon
{
private:
protected:
public:
	sf2drt_coordinate_t points[3];

	sf2drt_equilateral_center_tip(sf2drt_coordinate_t center, sf2drt_coordinate_t tip) : sf2drt_polygon(3, points)
	{
		// Compute where to put the points, then add them to the base polygon
	}
};

class sf2drt_rect_center_corner : public sf2drt_polygon
{
private:
protected:
public:
	sf2drt_coordinate_t points[4];

	sf2drt_rect_center_corner(sf2drt_coordinate_t center, sf2drt_coordinate_t corner) : sf2drt_polygon(4, points)
	{
		// Compute where to put the points, then add them to the base polygon
		points[0] = corner;

		double x, y;

		x = (double)(2*center.x - corner.x);
		if(x < 0){ x = 0; }
		points[1] = {(sf2drt_extent_t)x, corner.y};

		y = (double)(2*center.y - corner.y);
		if(y < 0){ y = 0; }
		points[2] = {(sf2drt_extent_t)x, (sf2drt_extent_t)y};

		points[3] = {corner.x, (sf2drt_extent_t)y};
	}
};

class sf2drt_rect_2corner : public sf2drt_polygon
{
private:
protected:
public:
	sf2drt_coordinate_t points[4];

	sf2drt_rect_2corner(sf2drt_coordinate_t corner1, sf2drt_coordinate_t corner2) : sf2drt_polygon(4, points)
	{
		// Compute where to put the points, then add them to the base polygon
		points[0] = corner1;
		points[1] = {corner2.x, corner1.y};
		points[2] = corner2;
		points[3] = {corner1.x, corner2.y};
	}
};



#endif /* SF_2DRAYTRACING_H */