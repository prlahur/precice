#pragma once

#include <set>

namespace precice {
namespace utils {

/**
 * @brief Manages a set of unique IDs.
 */
class ManageUniqueIDs
{
public:

   /**
    * @brief Returns the next free, i.e. unique, ID.
    */
   int getFreeID ();

   /**
    * @brief Inserts an ID which has to be unique.
    *
    * The inserted ID has to be different to all other IDs inserted and obtained
    * from getFreeID().
    */
   bool insertID ( int id );

   /**
    * @brief Resets all retrieved and inserted IDs.
    */
   void resetIDs ();

private:

   // @brief Stores all used IDs.
   std::set<int> _ids;

   // @brief Marks next ID to be given, from lower to higher values.
   int _lowerLimit = 0;
};

}} // namespace precice, utils
