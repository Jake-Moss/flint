/*
    Copyright (C) 2016 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include <stdlib.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"

int fmpz_mpoly_equal_fmpz(const fmpz_mpoly_t poly,
                                    const fmpz_t c, const fmpz_mpoly_ctx_t ctx)
{
   slong N, i;

   if (fmpz_is_zero(c))
      return poly->length == 0;

   if (poly->length != 1)
      return 0;

   N = mpoly_words_per_exp(poly->bits, ctx->minfo);

   for (i = 0; i < N; i++)
   {
      if (poly->exps[i] != 0)
         return 0;
   }

   return fmpz_equal(poly->coeffs + 0, c);
}
