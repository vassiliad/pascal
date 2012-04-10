#include <stdio.h>
#include "expressions.h"
#include "dead_code.h"

statement_t *dead_code_elimination(statement_t *root, scope_t *scope)
{
	statement_t *p, *t;
	
	p = root;
	while ( p ) {
		switch ( p->type ) {
			case ST_If:
			{
				statement_if_t n = p->_if;
				constant_t constant;

				// Attempt to evaluate the condition at compile time
				if ( expression_evaluate(n.condition, &constant, scope) 
						== Success ) {

					// if the condition is always true replace IF with TRUE block
					if ( constant.bconst == True ) {
						if ( p->prev )
							p->prev->next = n._true;

						n._true->prev = p->prev;

						for ( t=n._true; t->next; t=t->next );

						t->next = p->next;

						if ( p->next )
							p->next->prev = t;

						p = n._true;
						continue;
					}	else {
					// if the condition is always false replace IF with FALSE block
						if ( p->prev )
							p->prev->next = n._false;
						n._false->prev = p->prev;
						p = n._false;

						continue;
					}	
				} else {
					dead_code_elimination(n._true, scope);
					dead_code_elimination(n._false, scope);
				}
			}
			break;

			case ST_While:
			{
				statement_while_t n = p->_while;
				constant_t constant;

				// Attempt to evaluate the condition at compile time
				if ( expression_evaluate(n.condition, &constant, scope) 
						== Success ) {
					
					// this while's loop will never be executed, remove it
					if ( constant.bconst == False ) {
						if ( p->prev )
							p->prev->next = p->next;
						if ( p->next )
							p->next->prev = p->prev;
					} else {
					// this while will always execute
					// TODO this will be done later
					}
				} else {
					dead_code_elimination(n.loop, scope);
				}
			}
			break;

			case ST_For:
			{
				statement_for_t n = p->_for;
				dead_code_elimination(n.loop, scope);
			}
			break;

			case ST_With:
			{
				dead_code_elimination(p->with.statement ,scope);
			}
			break;

			case ST_Assignment:
			break;

			case ST_Call:
			break;

			case ST_Case:
			break;
		}

		p = p->next;
	}
	

	return root;
}
