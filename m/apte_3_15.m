axis equal;
hold on;
grid on;

block_x=[0 0 4224.76 4224.76 0 ];
block_y=[0 12674.3 12674.3 0 0 ];
fill(block_x,block_y,'r','facealpha',0.5);
block_x=[0 0 1826 1826 0 ];
block_y=[9518 12664 12664 9518 9518 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(913, 11091, 'cc_11');

block_x=[1832 1832 3658 3658 1832 ];
block_y=[9518 12664 12664 9518 9518 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(2745, 11091, 'cc_12');

block_x=[286 286 2112 2112 286 ];
block_y=[3186 6332 6332 3186 3186 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1199, 4759, 'cc_13');

block_x=[2118 2118 3944 3944 2118 ];
block_y=[0 3146 3146 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3031, 1573, 'cc_14');

block_x=[286 286 2118 2118 286 ];
block_y=[0 3186 3186 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1202, 1593, 'cc_21');

block_x=[2118 2118 3950 3950 2118 ];
block_y=[3146 6332 6332 3146 3146 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3034, 4739, 'cc_22');

block_x=[2118 2118 3950 3950 2118 ];
block_y=[6332 9518 9518 6332 6332 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3034, 7925, 'cc_23');

block_x=[0 0 1832 1832 0 ];
block_y=[6332 9518 9518 6332 6332 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(916, 7925, 'cc_24');

block_x=[0 0 286 286 0 ];
block_y=[0 826 826 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(143, 413, 'clk');

