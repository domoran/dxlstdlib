/*
#include <C:\Programme\Programmer's Notepad\Doors\find_current_module.inc>

print fullName current Module
*/


int findCurrent_OpenModulesCount = 0
int findCurrent_count = 0
Skip findCurrent_modules = create()
Module findCurrent_mod
DBE findCurrent_list


void closeAction(DB dialog)
{
	int pos = get findCurrent_list
	if (find(findCurrent_modules , pos, findCurrent_mod)) {
		current = findCurrent_mod 
		// print "Yalla" fullName findCurrent_mod
	}
	release dialog
	hide dialog
	// destroy dialog
}

for findCurrent_mod in database do {
	if (isVisible findCurrent_mod) {
		put(findCurrent_modules, findCurrent_OpenModulesCount, findCurrent_mod)
		findCurrent_OpenModulesCount ++		
	}
}

if (findCurrent_OpenModulesCount == 1) {
	find(findCurrent_modules,0,findCurrent_mod)
	current = findCurrent_mod
} else if (findCurrent_OpenModulesCount > 1) {
	string findCurrent_modValues[findCurrent_OpenModulesCount]
	
	for findCurrent_count in 0:findCurrent_OpenModulesCount-1 do {
		find(findCurrent_modules, findCurrent_count, findCurrent_mod)
		findCurrent_modValues[findCurrent_count] = name findCurrent_mod		
	}
	

	DB findCurrent_openDiag = create "More than one current Module found"
	// label(findCurrent_openDiag, )
	findCurrent_list = list (findCurrent_openDiag, "Please select the module, that should be used as current Module:", 
					400, 12, findCurrent_modValues)
					
	close (findCurrent_openDiag, true, closeAction)
	block findCurrent_openDiag
    destroy findCurrent_openDiag
}


delete findCurrent_modules
