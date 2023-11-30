axis equal;
hold on;
grid on;

block_x=[0 0 7317.5 7317.5 0 ];
block_y=[0 7317.5 7317.5 0 0 ];
fill(block_x,block_y,'r','facealpha',0.5);
block_x=[0 0 3146 3146 0 ];
block_y=[5304 7130 7130 5304 5304 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1573, 6217, 'cc_11');

block_x=[3664 3664 5490 5490 3664 ];
block_y=[0 3146 3146 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(4577, 1573, 'cc_12');

block_x=[5490 5490 7316 7316 5490 ];
block_y=[0 3146 3146 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(6403, 1573, 'cc_13');

block_x=[3146 3146 6292 6292 3146 ];
block_y=[5304 7130 7130 5304 5304 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(4719, 6217, 'cc_14');

block_x=[0 0 3186 3186 0 ];
block_y=[3472 5304 5304 3472 3472 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1593, 4388, 'cc_21');

block_x=[3186 3186 6372 6372 3186 ];
block_y=[3186 5018 5018 3186 3186 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(4779, 4102, 'cc_22');

block_x=[0 0 1832 1832 0 ];
block_y=[0 3186 3186 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(916, 1593, 'cc_23');

block_x=[1832 1832 3664 3664 1832 ];
block_y=[0 3186 3186 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(2748, 1593, 'cc_24');

block_x=[1832 1832 2658 2658 1832 ];
block_y=[3186 3472 3472 3186 3186 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(2245, 3329, 'clk');

