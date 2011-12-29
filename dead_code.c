#include <stdio.h>
#include "dead_code.h"

statement_t *dead_code_elimination(statement_t *root, scope_t *scope)
{
	statement_t *p;
	for( p = root; p; p=p->next ) {
		switch ( p->type ) {
			case ST_If:
			{
				statement_if_t n = p->_if;

				// Attempt to evaluate the condition at compile time
				if ( n.condition->type == ET_Constant ) {
					constant_t constant = n.condition->constant;
					if ( constant.bconst == True ) {
						if ( p->prev )
							p->prev->next = n._true;
						n._true->prev = p->prev;
						p=n._true;
					}	else {
						if ( p->prev )
							p->prev->next = n._false;
						n._false->prev = p->prev;
						p = n._false;
					}	
				} else {
					dead_code_elimination(n._true, scope);
					dead_code_elimination(n._false, scope);
				}
			}
			break;

			case ST_While:
			break;

			case ST_For:
			break;

			case ST_With:
			break;

			case ST_Assignment:
			break;

			case ST_Call:
			break;

			case ST_Case:
			break;
		}
	}
	

	return root;
}
