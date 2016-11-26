#!/usr/bin/python

import matplotlib.pyplot as plt, matplotlib.patches as patches

fig = plt.figure()

def rectangle(sp, x, y, w, h, colr, patt):
   sp.add_patch(patches.Rectangle((x, y), w, h, hatch=patt, facecolor=colr, edgecolor='k'))

def layout(mc, nrows, ncols, width, height, t):
  wt = 10 
  side = (width - (t * (ncols - 1)))/ncols

  print "Side length: ", side
  print "%2d plates of length %5.2f" % (nrows * (ncols - 1), side)
  print "%2d plates of length %5.2f" % (nrows * 2, side + t/2)
  print "%2d plates of length %5.2f" % (nrows * (ncols -2) , side + t)

  sp = fig.add_subplot(mc, aspect='equal')
  rectangle(sp, width, 0, wt, height, '#e0e0e0', '/')
  rectangle(sp, -wt,   0, wt, height, '#e0e0e0', '/')

  for ny in range (nrows):
    y1 = (side + t) * ny 
    y2 = y1 + side 
    for nx in range(ncols - 1):
      x1 = (nx + 1) * side + nx * t
      rectangle(sp, x1, y1, t, side, 'r', '')

    x2 = 0 
    for nx in range(ncols):
      w = side + t
      c = 'b'
      if nx == 0 or nx == (ncols - 1):
        w = side + t/2 
        c='g'
      rectangle(sp, x2, y2, w, t, c, '')
      x2 = x2 + w
  plt.ylim([0, height])
  plt.xlim([-10, width+10])

t = 5.0
t2=7.5
h = 209.0
b = 190.0
#layout(221, 4, 5, b, h, t)
#layout(222, 5, 5, b, h, t)
#layout(223, 3, 4, b, h, t)
#layout(224, 4, 4, b, h, t)
layout(221, 5, 5, b, h, t)
layout(222, 5, 5, b, h, t2)
layout(223, 4, 4, b, h, t)
layout(224, 4, 4, b, h, t2)
plt.show()
