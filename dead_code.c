#include <stdio.h>
#include "dead_code.h"

statement_t *dead_code_elimination(statement_t *root, scope_t *scope)
{
	statement_t *p;
	printf("ROOT: %p\n", root);
	for( p = root; p; p=p->next ) {
		printf("%d\n", p->type);
		switch ( p->type ) {
			case ST_If:
			{
				statement_if_t n = p->_if;

				// Attempt to evaluate the condition at compile time
				if ( n.condition->type == ET_Constant ) {
					printf("Nai re manga!\n");
				} else {
					printf("SKATA! (%d)\n", n.condition->type);
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
