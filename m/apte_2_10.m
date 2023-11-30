axis equal;
hold on;
grid on;

block_x=[0 0 5060.52 5060.52 0 ];
block_y=[0 10121 10121 0 0 ];
fill(block_x,block_y,'r','facealpha',0.5);
block_x=[0 0 1826 1826 0 ];
block_y=[3186 6332 6332 3186 3186 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(913, 4759, 'cc_11');

block_x=[1832 1832 4978 4978 1832 ];
block_y=[0 1826 1826 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3405, 913, 'cc_12');

block_x=[1826 1826 4972 4972 1826 ];
block_y=[3658 5484 5484 3658 3658 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3399, 4571, 'cc_13');

block_x=[0 0 3146 3146 0 ];
block_y=[6332 8158 8158 6332 6332 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1573, 7245, 'cc_14');

block_x=[1832 1832 5018 5018 1832 ];
block_y=[1826 3658 3658 1826 1826 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3425, 2742, 'cc_21');

block_x=[0 0 3186 3186 0 ];
block_y=[8158 9990 9990 8158 8158 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(1593, 9074, 'cc_22');

block_x=[0 0 1832 1832 0 ];
block_y=[0 3186 3186 0 0 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(916, 1593, 'cc_23');

block_x=[3186 3186 5018 5018 3186 ];
block_y=[6310 9496 9496 6310 6310 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(4102, 7903, 'cc_24');

block_x=[3146 3146 3432 3432 3146 ];
block_y=[5484 6310 6310 5484 5484 ];
fill(block_x,block_y,'c','facealpha',0.5);
text(3289, 5897, 'clk');

