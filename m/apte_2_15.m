axis equal;
hold on;
grid on;

block_x=[0 0 5174.26 5174.26 0 ];
block_y=[0 10348.5 10348.5 0 0 ];
fill(block_x,block_y,'r','facealpha',0.5);
block_x=[3186 3186 5012 5012 3186 ];
block_y=[6310 9456 9456 6310 6310 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(4099, 7883, 'cc_11');

block_x=[1832 1832 4978 4978 1832 ];
block_y=[1832 3658 3658 1832 1832 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3405, 2745, 'cc_12');

block_x=[1826 1826 4972 4972 1826 ];
block_y=[3658 5484 5484 3658 3658 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3399, 4571, 'cc_13');

block_x=[0 0 1826 1826 0 ];
block_y=[3186 6332 6332 3186 3186 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(913, 4759, 'cc_14');

block_x=[0 0 1832 1832 0 ];
block_y=[0 3186 3186 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(916, 1593, 'cc_21');

block_x=[0 0 3186 3186 0 ];
block_y=[8164 9996 9996 8164 8164 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1593, 9080, 'cc_22');

block_x=[1832 1832 5018 5018 1832 ];
block_y=[0 1832 1832 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3425, 916, 'cc_23');

block_x=[0 0 3186 3186 0 ];
block_y=[6332 8164 8164 6332 6332 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1593, 7248, 'cc_24');

block_x=[3186 3186 3472 3472 3186 ];
block_y=[5484 6310 6310 5484 5484 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3329, 5897, 'clk');

