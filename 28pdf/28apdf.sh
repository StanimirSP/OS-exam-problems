#!/bin/bash

grep -E "^[+-]?[0-9]+$" | \
  sed -E 's/^([+-]?)0*(.)/\1\2/g' | awk 'function abs(n) { return n<0? -n: n; }
							   BEGIN { max=0; hasPositive=0; hasNegative=0; }
						  	   { if(max<abs($1)) { max=abs($1); hasPositive=0; hasNegative=0; }
							  	 if(max==$1) hasPositive=1;
								 if(-max==$1) hasNegative=1;
							   }
						       END { if(hasNegative && max!=0) print -max;
									 if(hasPositive) print max;
								   }'  | sed 's/^+//'
