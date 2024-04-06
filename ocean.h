/*!****************************************************************************************************************************************************************************************************************************************************************
\file       ocean.h
\par        Author:          Najih
\par        Email:           n.hajasheikallaudin
\par        Course:          RSE1202    
\par        Section:         RSE
\par        Tutorial:        Assignment 5
\par        File created on: 10/02/2024
\brief      battle ship game
 
\par        File information:
            This file is contains 9 function definitions to called by main in q-driver.
            Functions:
            - Ocean::Ocean(int num_boats, int x_size, int y_size);                               // constructor to create the ocean grid using x and y size and allocate memory for the ocean grid and for the array of boats to be place in ocean grid
            - BoatPlacement Ocean::PlaceBoat(Boat const& boat) const ;                           // to place boats into the ocean grid and updating the boats stats in the ocean struct object.
            - ShotResult Ocean::TakeShot(Point const& coordinate);                               // to identify stats of the shots fired in the ocean grid and up date object in structure shotstats in structure ocean
            - ShotStats Ocean::GetShotStats() const;                                             // to return all the stats(hits, sunk, misses and duplicates) stord in the struct ShotStats in struct ocean 
            - Point Ocean::GetDimensions() const;                                                // Assuming width and height are member variables
            - int const* Ocean::GetGrid() const;                                                 // Assuming grid is a member variable of type int
            - Ocean::~Ocean();                                                                   // destructor to delete/free allocated memory for the ocean struct objects
            - void Dump(const Ocean &ocean,int field_width,  bool extraline, bool showboats);    // to print the physical representation of the grid 
 (user made)- bool Ocean::IsValidCoordinate(Point const& coordinate) const;                      // to check if cordinated are within the bounders of the grid
******************************************************************************************************************************************************************************************************************************************************************/
#ifndef OCEAN_H
#define OCEAN_H

#include <vector>

namespace HLP2 {
  namespace WarBoats {
    
    inline int const BOAT_LENGTH {4};                                                            //!< Length of a boat
    inline int const HIT_OFFSET {100};                                                           //!< Add this to the boat ID

    class Ocean;                                                                                 //!< Forward declaration for the Ocean 
    enum Orientation   { oHORIZONTAL, oVERTICAL };
    enum ShotResult    { srHIT, srMISS, srDUPLICATE, srSUNK, srILLEGAL };
    enum DamageType    { dtOK = 0, dtBLOWNUP = -1 };
    enum BoatPlacement { bpACCEPTED, bpREJECTED };

    struct Point {                                                                               //! A coordinate in the Ocean
        int x;                                                                                   //!< x-coordinate (column)
        int y;                                                                                   //!< y-coordinate (row)
    };
    struct Boat {                                                                                //! A boat in the Ocean
        int hits;                                                                                //!< Hits taken so far
        int ID;                                                                                  //!< Unique ID 
        Orientation orientation;                                                                 //!< Horizontal/Vertical
        Point position;                                                                          //!< x-y coordinate (left-top)
    };
    struct ShotStats {                                                                           //! Statistics of the "game"
        int hits;                                                                                //!< The number of boat hits
        int misses;                                                                              //!< The number of boat misses
        int duplicates;                                                                          //!< The number of duplicate (misses/hits)
        int sunk;                                                                                //!< The number of boats sunk
    };

    class Ocean {                                                                                //! The attributes of the ocean
    public:
        Ocean(int num_boats, int x_size, int y_size);                                            // constructor
        ~Ocean();                                                                                // destructor
        ShotResult TakeShot(Point const& coordinate);
        BoatPlacement PlaceBoat(Boat const& boat) const ;
        ShotStats GetShotStats() const;
        void Dump(int field_width = 4,  bool extraline = false, bool showboats = true) const;

        // additional functions required of ocean class
        Point GetDimensions() const;
        int const* GetGrid() const;
        bool IsValidCoordinate(Point const& coordinate) const;
    
    private:                                                                                     //! The attributes of the ocean
        int  *grid;                                                                              //!< The 2D ocean 
        int num_boats;                                                                           //!< Number of boats in the ocean
        int x_size;                                                                              //!< Ocean size along x-axis
        int y_size;                                                                              //!< Ocean size along y-axisBoat 
        Boat *boats;                                                                             //!< Boat information
        Point location;                                                                          //!< The dynamic array of boats
        ShotStats stats;                                                                         //!< Status of the attack
    };
  }                                                                                              // namespace WarBoats
}                                                                                                // namespace HLP2
#endif // OCEAN_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
