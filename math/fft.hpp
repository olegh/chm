#if !defined(DCF02802_2F41_44ff_91EA_D6B35DF79591)
# define DCF02802_2F41_44ff_91EA_D6B35DF79591

#include <cmath>
#include <complex>
#include <iterator>

namespace math{ 
namespace privateImpl{
 
inline unsigned
bitReverse( unsigned x, int log2n )
{
  unsigned n = 0;
  for( int i = 0; i < log2n; ++i )
  {
    n <<= 1;
    n |= (x & 1);
    x >> 1;
  }

  return n;
}//bitReverse( unsigned x, int log2n )

} //namespace privateImpl{
} //namespace math{

#endif //# define DCF02802_2F41_44ff_91EA_D6B35DF79591