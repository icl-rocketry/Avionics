#include "barofilter.h"


BaroFilter::BaroFilter(float alpha):
_alpha(alpha),
_prevFilterAlt(0.0) 
{}

float BaroFilter::getFilterAlt(float alt) 
{
    _prevFilterAlt = ( _alpha*alt ) + ( (1-_alpha)*_prevFilterAlt );
    return _prevFilterAlt;
}

