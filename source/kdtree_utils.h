#ifndef KDTREE_UTILS_H
#define KDTREE_UTILS_H

#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <cstdlib>

#include "kdtree_types.h"
#include "kdtree_constants.h"
#include "kdtree_hyperplane.h"

// @Purpose
//
// This struct provides simple mathematical utility functions in a
// separate, easily verifiable package

namespace datastructures {

struct Utils {
    // PRIMARY INTERFACE
    template< typename T >
    static size_t axisOfHighestVariance( const Types::Points< T >& points );
        // Given a set of equally dimensional points finds an
        // with highest variance

    template< typename T >
    static T medianValueInAxis( const Types::Points< T >& points,
                                const size_t axis );
        // Given a set of equally dimensional points and a specific
        // axis find a median value of all the points on that axis

    template< typename T >
    static Types::AxisMinMax< T >
    minMaxPerAxis( const Types::Points< T >& points );
        // Given a set of equally dimensional points find min and max value
        // for each axis

    template< typename T >
    static double
    distance( const Types::Point< T >& p1, const Types::Point< T >& p2 );
        // Computed distance between two points. Returns
        // KDTREE_INVALID_POINT_DISTANCE in case points are of different
        // cardinality

    template< typename T >
    static double
    distance( const Types::Point< T >& p, const KDHyperplane< T >& plane );
        // Computed distance between two points. Returns
        // KDTREE_INVALID_DISTANCE in case points are of different
        // cardinality
};

//============================================================================
//                  PRIMARY INTERFACE
//============================================================================

template< typename T >
size_t
Utils::axisOfHighestVariance( const Types::Points< T >& points )
{
    // Sanity
    if ( !points.size() )
    {
        return Constants::KDTREE_EMPTY_SET_VARIANCE;
    }

    const Types::AxisMinMax< T > axisMinMax =
            Utils::minMaxPerAxis< T >( points );


    size_t axis = 0;
    T largestVariance = std::abs( axisMinMax[ 0u ].second -
                                  axisMinMax[ 0u ].first );

    for ( size_t i = 1u; i < axisMinMax.size(); ++i )
    {
        const T curr =
                std::abs( axisMinMax[ i ].second - axisMinMax[ i ].first );
        if ( curr > largestVariance )
        {
            largestVariance = curr;
            axis = i;
        }
    }

    return axis;
}

template< typename T >
T
Utils::medianValueInAxis( const Types::Points< T >& points,
                          const size_t axis )
{
    // Sanity
    if ( !points.size() )
    {
        return Constants::KDTREE_EMPTY_SET_VARIANCE;
    }

    std::vector< T > values;
    values.reserve( points.size() );

    for ( typename Types::Points< T >::const_iterator it = points.cbegin();
          it != points.cend(); ++it )
    {
        // Sanity
        if ( ( *it ).size() <= axis)
        {
            return Constants::KDTREE_EMPTY_SET_VARIANCE;
        }

        values.push_back( ( *it )[ axis ] );
    }

    const size_t n = values.size() / 2;

    std::nth_element( values.begin(),
                      values.begin() + n,
                      values.end() );

    return values[ n ];
}

template< typename T >
Types::AxisMinMax< T >
Utils::minMaxPerAxis( const Types::Points< T >& points )
{
    Types::AxisMinMax< T >minMaxPerAxis;

    // Sanity
    if ( !points.size() )
    {
        return minMaxPerAxis;
    }

    // Prime the mix/max
    typename Types::Points< T >::const_iterator it = points.cbegin();

    minMaxPerAxis.reserve( ( *it ).size() );
    for ( size_t i = 0u; i < ( *it ).size(); ++i )
    {
        minMaxPerAxis.push_back( std::pair< T, T >( ( *it )[ i ], ( *it )[ i ] ) );
    }
    ++it;

    // Find min/max value per axis
    for ( ;it != points.cend(); ++it )
    {
        for ( size_t i = 0u; i < ( *it ).size(); ++i )
        {
            if ( minMaxPerAxis[ i ].first > ( *it )[ i ] )
            {
                minMaxPerAxis[ i ].first = ( *it )[ i ];
            }

            if ( minMaxPerAxis[ i ].second < ( *it )[ i ] )
            {
                minMaxPerAxis[ i ].second = ( *it )[ i ];
            }
        }
    }

    return minMaxPerAxis;
}

template< typename T >
double
Utils::distance( const Types::Point< T >& p1, const Types::Point< T >& p2 )
{
    // Sanity
    if ( p1.size() != p2.size() )
    {
        return Constants::KDTREE_INVALID_DISTANCE;
    }

    double dist2 = 0.0L;

    typename Types::Point< T >::const_iterator it1 = p1.cbegin();
    typename Types::Point< T >::const_iterator it2 = p2.cbegin();
    for ( ;it1 != p1.cend() && it2 != p2.cend(); ++it1, ++it2 )
    {
        double temp = ( *it1 ) - ( *it2 );
        dist2 += temp * temp;
    }

    return sqrt( dist2 );
}

template< typename T >
double
Utils::distance( const Types::Point< T >& p, const KDHyperplane< T >& plane )
{
    // Sanity
    if ( p.size() <= plane.hyperplaneIndex() )
    {
        return Constants::KDTREE_INVALID_DISTANCE;
    }

    return std::abs( p[ plane.hyperplaneIndex() ] - plane.value() ) ;
}

} // namespace datastructures

#endif //KDTREE_UTILS_H
