#ifndef KDTREE_NODE_H
#define KDTREE_NODE_H

#include <iostream>
#include <memory>
#include <sstream>

#include "kdtree_types.h"
#include "kdtree_constants.h"

namespace datastructures {

// PURPOSE:
//
// A class that defines individual notes within a KD-Tree
//
template< typename T >
class KDNode {
public:

    // CREATORS
    KDNode();
        // Default constructor

    KDNode( const size_t                           hyperplaneIndex,
            const T                                value,
            const std::shared_ptr< KDNode< T > >&  left,
            const std::shared_ptr< KDNode< T > >&  right,
            const size_t                           leafIndex );
        // Constructor

    KDNode( const KDNode& other );
        // Copy constructor, calls copy().

    virtual ~KDNode();
        // Destructor

    // OPERATORS
    KDNode& operator=( const KDNode& other );
        // Assignment operator. Calls copy.
        // Note that this operator performs a shallow copy as its
        // use is targeted at containers.

    bool operator==( const KDNode& other ) const;
        // Equality. Calls equals.

    bool operator!=( const KDNode& other ) const;
        // Non-equality. Calls equals.

    // PRIMARY INTERFACE
    const size_t hyperplaneIndex() const;
        // Returns index of the hyperplane defined by this node

    const T value() const;
        // Return value of this node - i.e. position of the hyperplane
        // defined by this node.
        // Note that value of this type is not set by default dtor

    std::shared_ptr< KDNode< T > > left() const;
        // Return shared pointer to the left subtree

    std::shared_ptr< KDNode< T > > right() const;
        // Return shared pointer to the right subtree

    const size_t leafIndex() const;
        // Return index of the point stored in KDTree that this node
        // represents. Note that non-leaf nodes will return
        // Defaults::KDTREE_NONLEAF_INDEX

    // MANIPULATORS
    void copy( const KDNode& other );
        // Copies the value of other into this

    // ACCESSORS
    bool equals( const KDNode& other ) const;
        // Worker for equality - call this in child classes when overloading
        // == and != operator

    std::ostream& print( std::ostream& out ) const;
        // Prints the contents of the KDNode object in a easy to read
        // format

private:
    size_t                             m_hyperplaneIndex;
        // Defines the index of the hyperplane

    T                                  m_value;
        // Position of the hyperplane, left subtree is less or equal, right
        // subtree is greater

    std::shared_ptr< KDNode< T > >     m_left;
        // Left subtree

    std::shared_ptr< KDNode< T > >     m_right;
        // Right subtree

    size_t                             m_leafIndex;
        // Index of the point in the KDTree internal structure. Note that
        // this value is set Defaults::KDTREE_NONLEAF_INDEX for all
        // non-leaf nodes.
};

// INDEPENDENT OPERATORS
template< typename T >
std::ostream& operator<<( std::ostream& lhs,
                          const KDNode< T >& rhs );

//============================================================================
//                  CREATORS
//============================================================================

template< typename T >
KDNode< T >::KDNode()
: m_hyperplaneIndex( Defaults::KDTREE_UNINITIALIZED_HYPERPLANE_INDEX )
, m_left(            NULL )
, m_right(           NULL )
, m_leafIndex(       Defaults::KDTREE_NONLEAF_INDEX )
{
    // nothing to do here
}

template< typename T >
KDNode< T >::KDNode( const size_t                           hyperplaneIndex,
                     const T                                value,
                     const std::shared_ptr< KDNode< T > >&  left,
                     const std::shared_ptr< KDNode< T > >&  right,
                     const size_t                           leafIndex )
: m_hyperplaneIndex( hyperplaneIndex )
, m_value(           value )
, m_left(            left)
, m_right(           right )
, m_leafIndex(       leafIndex )
{
    // nothing to do here
}

template< typename T >
KDNode< T >::KDNode( const KDNode& other )
{
    copy( other );
}

template< typename T >
KDNode< T >::~KDNode()
{
    // nothing to do here
}

//============================================================================
//                  OPERATORS
//============================================================================

template< typename T >
KDNode< T >&
KDNode< T >::operator=( const KDNode< T >& other )
{
    copy( other );
    return *this;
}

template< typename T >
bool
KDNode< T >::operator==( const KDNode< T >& other ) const
{
    return equals( other );
}

template< typename T >
bool KDNode< T >::operator!=(
        const KDNode< T >& other ) const
{
    return !equals( other );
}

//============================================================================
//                  PRIMARY INTERFACE
//============================================================================
template< typename T >
const size_t
KDNode< T >::hyperplaneIndex() const
{
    return m_hyperplaneIndex;
}

template< typename T >
const T
KDNode< T >::value() const
{
    return m_value;
}

template< typename T >
std::shared_ptr< KDNode< T > >
KDNode< T >::left() const
{
    return m_left;
}

template< typename T >
std::shared_ptr< KDNode< T > >
KDNode< T >::right() const
{
    return m_right;
}

template< typename T >
const size_t
KDNode< T >::leafIndex() const
{
    return m_leafIndex;
}

//============================================================================
//                  MANIPULATORS
//============================================================================

template< typename T >
void
KDNode< T >::copy( const KDNode< T >& other )
{
    m_hyperplaneIndex = other.hyperplaneIndex();
    m_value           = other.value();
    m_left            = other.left();
    m_right           = other.right();
    m_leafIndex       = other.leafIndex();
}

//============================================================================
//                  ACCESSORS
//============================================================================

template< typename T >
bool
KDNode< T >::equals(
        const KDNode< T >& other ) const
{
    return ( ( other.hyperplaneIndex() == m_hyperplaneIndex ) &&
             ( other.value()           == m_value           ) &&
             ( other.left()            == m_left            ) &&
             ( other.right()           == m_right           ) &&
             ( other.leafIndex()       == m_leafIndex       ) );
}

template< typename T >
std::ostream&
KDNode< T >::print( std::ostream& out ) const
{
    out << "KDNode:["
        << "hyperplane index = '" << std::dec << m_hyperplaneIndex << "', "
        << "value = '"            << std::dec << m_value           << "', "
        << "left ptr = '"         << std::hex << m_left            << "', "
        << "right ptr = '"        << std::hex << m_right           << "', "
        << "leaf index = '"       << std::hex << m_leafIndex       << "']";

    return out;
}

//============================================================================
//                  INDEPENDENT OPERATORS
//============================================================================
template< typename T >
std::ostream& operator<<( std::ostream& lhs,
                          const KDNode< T >& rhs )
{
    return rhs.print( lhs );
}

} // close namespace datastructures

#endif // KDTREE_NODE_H