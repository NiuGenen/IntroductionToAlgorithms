/* Variables */
var g1 >= 0;
var g2 >= 0;
var g3 >= 0;
var g4 >= 0;
var d >= 0;

/* Object function */
minimize z: d;

/* Constrains */
s.t. con1: g1 >= 1; 
s.t. con2: g1 <= 3; 
s.t. con3: g2 >= 4; 
s.t. con4: g2 <= 7; 
s.t. con5: g3 >= 8; 
s.t. con6: g3 <= 12; 
s.t. con7: g4 >= 14; 
s.t. con8: g4 <= 18; 
s.t. con9: g2 - g1 <= d; 
s.t. con10: g3 - g2 <= d; 
s.t. con11: g4 - g3 <= d; 

end;
