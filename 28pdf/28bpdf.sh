#!/bin/bash

grep -E "^[+-]?[0-9]+$" | \
 sed -E 's/^([+-]?)0*(.)/\1\2/g' | awk 'function sumDig(n,		sum, i) {
							  	  if(substr(n, 1, 1) !~ /[0-9]/) i=2;
								  else i=1;
								  sum=0;
								  for(; i<=length(n); ++i) sum+=substr(n, i, 1);
							      return sum;
 							   }
							   BEGIN { sum=-1; min=0; }
							   { s=sumDig($1);
								 if(sum<s) { sum=s; min=$1; }
								 if(sum==s && min>$1) min=$1;
							   }
							   END { print min; }'
							   
