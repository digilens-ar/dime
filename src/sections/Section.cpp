/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class DimeSection dime/sections/Section.h
  \brief The dimeSection class is the superclass for all \e section classes.
  
  Currently supported sections are:
  - Header   (dimeHeaderSection)
  - Classes  (dimeClassSection)
  - Tables   (dimeTableSection)
  - Blocks   (dimeBlockSection)
  - Entities (dimeEntititySection)
  - Objects  (dimeObjectSection)
*/

#include <dime/sections/Section.h>
#include <string.h>

#include <dime/sections/UnknownSection.h>
#include <dime/sections/EntitiesSection.h>
#include <dime/sections/HeaderSection.h>
#include <dime/sections/BlocksSection.h>
#include <dime/sections/TablesSection.h>
#include <dime/sections/ClassesSection.h>
#include <dime/sections/ObjectsSection.h>

/*!
  \fn int dimeSection::countRecords() const
  Returns the number of records in this section. 
*/

/*!
  Constructor
*/

DimeSection::DimeSection()
{
}

/*!
  Empty virtual destructor.
*/

DimeSection::~DimeSection()
{
}

/*!
  Static function used to create the correct section object
  from a text string.
*/

DimeSection*
DimeSection::createSection(const char* const sectionname)
{
	if (!strcmp(sectionname, "HEADER"))
		return new DimeHeaderSection;
	if (!strcmp(sectionname, "CLASSES"))
		return new DimeClassesSection;
	if (!strcmp(sectionname, "OBJECTS"))
		return new DimeObjectsSection;
	if (!strcmp(sectionname, "TABLES"))
		return new DimeTablesSection;
	if (!strcmp(sectionname, "BLOCKS"))
		return new DimeBlocksSection;
	if (!strcmp(sectionname, "ENTITIES"))
		return new DimeEntitiesSection;
	return new dimeUnknownSection(sectionname);
}

bool
DimeSection::isOfType(const int thetypeid) const
{
	return thetypeid == dimeSectionType ||
		DimeBase::isOfType(thetypeid);
}
