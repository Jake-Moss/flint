/*
    Copyright (C) 2020 Fredrik Johansson

    This file is part of Calcium.

    Calcium is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "arb_fmpz_poly.h"
#include "ca.h"
#include "ca_ext.h"

void
ca_get_acb_raw(acb_t res, const ca_t x, slong prec, ca_ctx_t ctx)
{
    slong field_index;
    ulong xfield;

    xfield = x->field;

    if (xfield == CA_FIELD_ID_QQ)
    {
        acb_set_fmpq(res, CA_FMPQ(x), prec);
        return;
    }

    if (xfield == CA_FIELD_ID_QQ_I)
    {
        const fmpz *n, *d;

        n = QNF_ELEM_NUMREF(CA_NF_ELEM(x));
        d = QNF_ELEM_DENREF(CA_NF_ELEM(x));

        if (fmpz_is_one(d))
        {
            arb_set_round_fmpz(acb_realref(res), n, prec);
            arb_set_round_fmpz(acb_imagref(res), n + 1, prec);
        }
        else
        {
            arb_fmpz_div_fmpz(acb_realref(res), n, d, prec);
            arb_fmpz_div_fmpz(acb_imagref(res), n + 1, d, prec);
        }
        return;
    }

    if (CA_IS_SPECIAL(x))
    {
        acb_indeterminate(res);
        return;
    }

    field_index = xfield;

    if (CA_FIELD_IS_NF(ctx->fields + field_index))
    {
        if (CA_FIELD_NF(ctx->fields + field_index)->flag & NF_LINEAR)
            flint_abort();

        ca_ext_get_acb_raw(res, CA_FIELD_EXT_ELEM(ctx->fields + field_index, 0), prec, ctx);

        if (CA_FIELD_NF(ctx->fields + field_index)->flag & NF_QUADRATIC)
        {
            _arb_fmpz_poly_evaluate_acb(res, QNF_ELEM_NUMREF(CA_NF_ELEM(x)), 2, res, prec);
            acb_div_fmpz(res, res, QNF_ELEM_DENREF(CA_NF_ELEM(x)), prec);
        }
        else
        {
            _arb_fmpz_poly_evaluate_acb(res, NF_ELEM_NUMREF(CA_NF_ELEM(x)), NF_ELEM(CA_NF_ELEM(x))->length, res, prec);
            acb_div_fmpz(res, res, NF_ELEM_DENREF(CA_NF_ELEM(x)), prec);
        }
    }
    else
    {
        acb_ptr v;
        slong i, n;

        n = CA_FIELD_LENGTH(ctx->fields + field_index);

        if (n == 1)
        {
            ca_ext_get_acb_raw(res, CA_FIELD_EXT_ELEM(ctx->fields + field_index, 0), prec, ctx);
            fmpz_mpoly_q_evaluate_acb(res, CA_MPOLY_Q(x), res, prec, ctx->mctx + 0);
        }
        else
        {
            v = _acb_vec_init(n);

            for (i = 0; i < n; i++)
                ca_ext_get_acb_raw(v + i, CA_FIELD_EXT_ELEM(ctx->fields + field_index, i), prec, ctx);

            fmpz_mpoly_q_evaluate_acb(res, CA_MPOLY_Q(x), v, prec, CA_FIELD_MCTX(ctx->fields + field_index, ctx));

            _acb_vec_clear(v, n);
        }
    }
}

