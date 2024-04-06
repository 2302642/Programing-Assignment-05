/*!****************************************************************************************************************************************************************************************************************************************************************
\file       ocean.cpp
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
#include "ocean.h"
#include <iostream> // std::cout
#include <iomanip>  // std::setw    

namespace HLP2 {
  namespace WarBoats {
//###############################################################################################################################################################################################################################################################################################################################
    /* ALL STUDENT IMPLEMENTATION GOES HERE 
    **************************************************************************************************************
      \brief:               Prints the grid (ocean) to the screen.
      \param ocean:         The Ocean to print
      \param field_width:   How much space each position takes when printed.
      \param extraline:     If true, an extra line is printed after each row. If false, no extra line is printed.
      \param showboats:     If true, the boats are shown in the output. (Debugging feature)
    ***************************************************************************************************************/
//===============================================================================================================================================================================================================================================================================================================================
//|                                                                         OCEAN CREATION SECTION                                                                                                                                                                                                                              |
//===============================================================================================================================================================================================================================================================================================================================
    // to create the ocean grid using x and y size and allocate memory for the ocean grid and for the array of boats to be place in ocean grid
    Ocean::Ocean(int num_boats, int x_size, int y_size) : grid{ new int[x_size * y_size]{} }, boats{ new Boat[num_boats]{} }, location{ x_size, y_size },  stats{} {}     // constructor to create the ocean grid using x and y size and allocate memory for the ocean grid and for the array of boats to be place in ocean grid
//===============================================================================================================================================================================================================================================================================================================================
//|                                                                         BOAT PLACEMENT SECTION                                                                                                                                                                                                                              |
//===============================================================================================================================================================================================================================================================================================================================
    BoatPlacement Ocean::PlaceBoat(Boat const& boat) const {                                                                                                              // to place boats into the ocean grid and updating the boats stats in the ocean struct object.

        if (!IsValidCoordinate(boat.position)) { return BoatPlacement::bpREJECTED; }                                                                                      // check if the boat is outside the ocean
        int start, end;
        if (boat.orientation == oHORIZONTAL) {                                                                                                                            // identifying the orientation of the boat
            start = boat.position.x; end = boat.position.x + BOAT_LENGTH; if (end > location.x) { return BoatPlacement::bpREJECTED; }                                                                                                   // checking for out of bound of ocean grid if it is then reject
        }
        else if (boat.orientation == oVERTICAL) {                                                                                                                         // identifying the orientation of the boat
            start = boat.position.y;                                                                                                                                      // start identifying the last posible vertical space that the boat can cover in the ocean grid
            end = boat.position.y + BOAT_LENGTH;                                                                                                                          // end identifying the maxium space of ocean grid (one dimentional array)
            if (end > location.y) { return BoatPlacement::bpREJECTED; }                                                                                                   // checking for out of bound of ocean grid if it is then reject
        }
        else { return BoatPlacement::bpREJECTED; }                                                                                                                        // checking for out of bound of ocean grid if it is then reject

        for (int i = start; i < end; ++i) {
            int gridIndex = (boat.orientation == oHORIZONTAL) ? i + boat.position.y * location.x : boat.position.x + i * location.x;
            if (grid[gridIndex] != DamageType::dtOK) { return BoatPlacement::bpREJECTED; }                                                                                // reject placement of boat in that grid coordinates if that coordinates is occupied
        }
        for (int i = start; i < end; ++i) {
            int gridIndex = (boat.orientation == oHORIZONTAL) ? i + boat.position.y * location.x : boat.position.x + i * location.x;                                      // identifying and assigning the position and orientation of the boat current boat id 
            grid[gridIndex] = boat.ID;                                                                                                                                    // placeing the boat in the ocean grid in the specified location
        }
        boats[boat.ID - 1] = boat;                                                                                                                                        // identifying and assigning the current boat for this iteration
        boats[boat.ID - 1].hits = 0;                                                                                                                                      // identifying and updating the hit stats for the current boat for this iteration
        
        return BoatPlacement::bpACCEPTED;
    }
//===============================================================================================================================================================================================================================================================================================================================
//|                                                                          SHOT RESULT SECTION                                                                                                                                                                                                                                |
//===============================================================================================================================================================================================================================================================================================================================
    ShotResult Ocean::TakeShot(Point const& coordinate) {                                                                                                                 // to identify stats of the shots fired in the ocean grid and up date object in structure shotstats in structure ocean
    if (!IsValidCoordinate(coordinate)) { return srILLEGAL; }                                                                                                             // checking if the shot coordinates are within the range of ocean grid else return srILLEGAL

    int& position = grid[coordinate.x + coordinate.y * location.x];                                                                                                       // Calculate the location of where the boat is to be in the ocean grid

    if (position == dtOK) {                                                                                                                                               // checking if that location was already occupied by a boat
        position = dtBLOWNUP;                                                                                                                                             // updating that location to be blown up
        stats.misses++;                                                                                                                                                   // and updating missed stats 
        return srMISS;                                                                                                                                                    // return srMISS
    }

    if (position > 0 && position < HIT_OFFSET) {                                                                                                                          // loop through each section of the boat
          int boatIndex = position - 1;
          boats[boatIndex].hits++;                                                                                                                                        // increment boat hits
          if (boats[boatIndex].hits >= BOAT_LENGTH) {                                                                                                                     // checking if all the sections of the boat is hit, and increment sunk in the ocean grid by updating the stats and return srSUNK
              stats.sunk++;                                                                                                                                               // update sunk stats
              position += HIT_OFFSET;                                                                                                                                     // setting that the boat is hit at that coordinates
              stats.hits++;   return srSUNK;                                                                                                                              // else update hit stats and return srSUNK
          }
          else {
              position += HIT_OFFSET;                                                                                                                                     // setting that the boat is hit at that coordinates
              stats.hits++;   return srHIT;                                                                                                                               // else update hit stats and return srHIT
          }
        }
    stats.duplicates++;   return srDUPLICATE;                                                                                                                             // update duplicates stats and return srDUPLICATE
  }
//===============================================================================================================================================================================================================================================================================================================================
//|                                                                          SHOTS STATS SECTION                                                                                                                                                                                                                                |
//===============================================================================================================================================================================================================================================================================================================================
    ShotStats Ocean::GetShotStats() const { return stats;    }                                                                                                            // to return all the stats(hits, sunk, misses and duplicates) stord in the struct ShotStats in struct ocean 
//===============================================================================================================================================================================================================================================================================================================================
//|                                                                     OCEAN CLEARING/DELETING SECTION                                                                                                                                                                                                                         |
//===============================================================================================================================================================================================================================================================================================================================
    Ocean::~Ocean() {                                                                                                                                                     // to delete/free allocated memory for the ocean struct objects     
      delete[] grid;                                                                                                                                                      // deallocate memory allocated for the ocean grid
      delete[] boats;                                                                                                                                                     // deallocate memory allocated for the array of boats
    }
//===============================================================================================================================================================================================================================================================================================================================
//|                                                                   OCEAN LAYOUT PRINTING SECTION (given)                                                                                                                                                                                                                     |
//===============================================================================================================================================================================================================================================================================================================================
    void Dump(const Ocean &ocean,int field_width,  bool extraline, bool showboats) {                                                                                     // to print the physical representation of the grid 
        int oceanWidth = ocean.GetDimensions().x;
        int oceanHeight = ocean.GetDimensions().y;
        
        for (int y = 0; y < oceanHeight; y++) {                                                                                                                           // for each row
            for (int x = 0; x < oceanWidth; x++) {                                                                                                                        // for each column
                int value = ocean.GetGrid()[y * oceanWidth + x];                                                                                                          // get value at x/y position            
                value = ( (value > 0) && (value < HIT_OFFSET) && (!showboats) ) ? 0 : value;                                                                              // is it a boat that we need to keep hidden?
                std::cout << std::setw(field_width) << value;
            }
            std::cout << "\n";
            if (extraline) { std::cout << "\n"; }
      }
    }
//===============================================================================================================================================================================================================================================================================================================================
//|           ADDITIONAL FUNCTIONS SECTION (compared to assignment 3 functions for COORDINATE BOUNDTY CHECKING, GET LOCATION OF BAOT AND GETING DIMENSIONS OF THE GRID)                                                                                                                                                         |
//===============================================================================================================================================================================================================================================================================================================================
    Point Ocean::GetDimensions() const    { return location; }                                                                                                            // Assuming width and height are member variables
    int const* Ocean::GetGrid() const     { return grid;     }                                                                                                            // Assuming grid is a member variable of type int
    bool Ocean::IsValidCoordinate(Point const& coordinate) const{ return coordinate.x >= 0 && coordinate.x < location.x && coordinate.y >= 0 && coordinate.y < location.y; }  // to check if cordinated are within the bounders of the grid
//###############################################################################################################################################################################################################################################################################################################################
  } // namespace WarBoats
} // namespace HLP2
