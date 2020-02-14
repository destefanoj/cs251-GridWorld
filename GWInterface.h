

#ifndef __GW_INTERFACE_H
#define __GW_INTERFACE_H

#include <vector>
#include <iostream>

/*
 */



class GWInterface {

  public:


    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     */
    virtual bool birth(int row, int col, int &id) = 0;


    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    virtual bool death(int personID) = 0;

    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    virtual bool move(int personID, int targetRow, int targetCol)= 0;

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     */
    virtual bool whereis(int personID, int &row, int &col) const = 0;

    /*
     * function: members
     * description:  creates an integer vector and populates it with
     *   members of given district (row,col).
     *
     *   vector is returned as a pointer.
     *
     *   If specified district is not valid a vector is still returned
     *     -- it is just empty (not to be confused with null).
     */
    virtual std::vector<int> * members(int row, int col)const = 0;

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     */
    virtual int population()const = 0;

    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     */
    virtual int population(int row, int col)const = 0;

    /*
     * function: num_rows
     * description:  returns number of rows in world
     */
    virtual int num_rows()const = 0;

    /*
     * function: num_cols
     * description:  returns number of columns in world
     */
    virtual int num_cols()const = 0;

    // destructor
    virtual ~GWInterface()=0;


  protected:
    
     // we need this for constructors in subclasses to work correctly...
     //   do not change!
     GWInterface(){ }
};

// another quirk thing - destructor for GWInterface class ..
//   needed for destructor of subclasses to work.
GWInterface::~GWInterface() {};


#endif

