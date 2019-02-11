#include "airfoil.h"
#include "environment.h"
#include <cmath>
#include <algorithm>
#include <iostream>

std::tuple<Vec2, double> Airfoil::getForceAndTorque(
    const Vec2& velocity, 
    double angle, 
    double airDensity,
    double height) const {
    
    Table::Entry coefficients = tableDefaultFlaps.get(getAngleOfAttack(velocity, angle));
    // TODO: add default constructor to avoid double function call
    if (flaps > 0) {
        // TODO: Ask guys how to make it better
        Table::Entry maxCoefficients = tableTakeOffFlaps.get(getAngleOfAttack(velocity, angle));
        coefficients.lift = coefficients.lift + ((maxCoefficients.lift - coefficients.lift) * flaps);
        coefficients.moment = coefficients.moment + ((maxCoefficients.moment - coefficients.moment) * flaps);
    }

    double dynamicPressure = 0.5 * airDensity * velocity.lengthSquared();


    double liftMagnitude = dynamicPressure * area * coefficients.lift * getGroundEffect(height);
    double dragMagnitude = dynamicPressure * area * coefficients.drag;

    Vec2 lift = velocity.tangent().normalized() * liftMagnitude;
    Vec2 drag = -velocity.normalized() * dragMagnitude;
    Vec2 force = lift + drag;

    double torque = dynamicPressure * area * chordLength * coefficients.moment;

    return {force, torque};
}


void Airfoil::setDeflection(double deflection) {
    this->deflection = std::max(-1.0, std::min(1.0, deflection));
}

void Airfoil::setFlaps(double flapsPosition) {
    // TODO: does better way exist?
    this->flaps = std::max(0.0, std::min(1.0, flapsPosition));
    
}