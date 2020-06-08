#define	Revision		1
#define	DEBUG			1

/*

	mini2.cpp			more M_ini stuff

	See "mini.h" for comments.
*/

#include	"mini.h"

#include	<stdio.h>
#include	<errno.h>
#include	<string.h>

/*
	class run-time type declarations
*/

M_CLSDECLARE_DERIVED(M_IniEntryIterator, M_Obj) ;

/*
	class M_IniEntryIterator

	(these methods should probably be split into a new module)
*/

/*
	M_IniEntryIterator:: public methods
*/

M_IniEntryIterator::~M_IniEntryIterator()
{
	// nothing to do
}

M_IniEntryIterator::M_IniEntryIterator(M_IniFile &iniFile, M_Str sectionName)
:	m_floc(0),
	m_IniFile(iniFile),
	m_SectionName(sectionName)
{
	// nothing to do
}

const M_Str &M_IniEntryIterator::GetEntryName()
{
	return(m_EntryName) ;
}

void M_IniEntryIterator::Rewind()
{
	m_floc.setFLOC(0) ;
}

M_SBOOL M_IniEntryIterator::Next()
{
	return(m_IniFile.getNextSectionName(m_SectionName, m_floc)) ;
}


// end of mini2.cpp
