//============================================================================
// @author  : sunquan
// @date   : 2019-09-25 18:44:22
//============================================================================

/**
 g++ main.cpp -lQuantLib -Wall -O0 -o hello
*/

#include "cash_bond.hpp"     //现券类
#include "outright_repo.hpp" //质押式回购类
#include "pledge_repo.hpp"   //买断式回购类


int main()
{
    CashBond::run();
    OutRightRepo::run();
    PledgeRepo::run();
    return 0;
}
