/*
    Copyright (C) 2017 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "nmod_mpoly.h"


void nmod_mpoly_degrees_si(slong * degs, const nmod_mpoly_t poly,
                                                    const nmod_mpoly_ctx_t ctx)
{
    mpoly_degrees_si(degs, poly->exps, poly->length, poly->bits, ctx->minfo);
}

slong nmod_mpoly_degree_si(const nmod_mpoly_t poly, slong var,
                                                    const nmod_mpoly_ctx_t ctx)
{
    slong * degs, ret;
    TMP_INIT;

    TMP_START;
    degs = (slong *) TMP_ALLOC(ctx->minfo->nvars*sizeof(slong));
    mpoly_degrees_si(degs, poly->exps, poly->length, poly->bits, ctx->minfo);
    ret = degs[var];

    TMP_END;
    return ret;
}

