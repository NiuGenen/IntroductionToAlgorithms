/* Variables */
var g1;
var g2;
var g3;
var g4;
var g5;
var d;

/* Object function */
minimize z: d;

/* Constrains */
s.t. con1:  10 - g1 <= 4; 
s.t. con2:  g1 - 10 <= 4; 
s.t. con3:  24 - g2 <= 4; 
s.t. con4:  g2 - 24 <= 4; 
s.t. con5:  35 - g3 <= 4; 
s.t. con6:  g3 - 35 <= 4; 
s.t. con7:  g4 - 46 <= 4; 
s.t. con8:  46 - g4 <= 4; 
s.t. con9:  g5 - 56 <= 4; 
s.t. con10: 56 - g5 <= 4; 
s.t. con11: g2 - g1 <= d; 
s.t. con12: g3 - g2 <= d; 
s.t. con13: g4 - g3 <= d; 
s.t. con14: g5 - g4 <= d; 

end;
