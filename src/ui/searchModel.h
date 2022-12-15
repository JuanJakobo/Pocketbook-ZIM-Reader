//------------------------------------------------------------------
// MinifluxModel.h
//
// Author:           JuanJakobo
// Date:             23.04.2021
// Description:      
//-------------------------------------------------------------------

#ifndef MINIFLUXMODEL
#define MINIFLUXMODEL

#include "model.h"
#include "entry.h"

#include <string>

struct SearchEntry : Entry
{
    SearchEntry(const zim::Entry &Eentry) : entry(Eentry){}
    zim::Entry entry;
};

#endif
