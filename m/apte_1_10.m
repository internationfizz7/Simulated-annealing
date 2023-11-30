axis equal;
hold on;
grid on;

block_x=[0 0 7156.66 7156.66 0 ];
block_y=[0 7156.66 7156.66 0 0 ];
fill(block_x,block_y,'r','facealpha',0.5);
block_x=[0 0 1826 1826 0 ];
block_y=[0 3146 3146 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(913, 1573, 'cc_11');

block_x=[3186 3186 5012 5012 3186 ];
block_y=[3664 6810 6810 3664 3664 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(4099, 5237, 'cc_12');

block_x=[5012 5012 6838 6838 5012 ];
block_y=[3664 6810 6810 3664 3664 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(5925, 5237, 'cc_13');

block_x=[1826 1826 3652 3652 1826 ];
block_y=[0 3146 3146 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(2739, 1573, 'cc_14');

block_x=[0 0 3186 3186 0 ];
block_y=[3146 4978 4978 3146 3146 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1593, 4062, 'cc_21');

block_x=[3652 3652 6838 6838 3652 ];
block_y=[0 1832 1832 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(5245, 916, 'cc_22');

block_x=[3652 3652 6838 6838 3652 ];
block_y=[1832 3664 3664 1832 1832 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(5245, 2748, 'cc_23');

block_x=[0 0 3186 3186 0 ];
block_y=[4978 6810 6810 4978 4978 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1593, 5894, 'cc_24');

block_x=[6838 6838 7124 7124 6838 ];
block_y=[0 826 826 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(6981, 413, 'clk');

