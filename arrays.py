def neighbors():
  for y in xrange(6):
      for x in xrange(4):
          if y % 2 == 0:
            neighbors = [(x, y - 1), (x + 1, y - 1), (x - 1, y), (x + 1, y), (x, y + 1), (x + 1, y + 1)]
          else:
            neighbors = [(x - 1, y - 1), (x, y - 1), (x - 1, y), (x + 1, y), (x - 1, y + 1), (x, y + 1)]
          neighbors = filter(lambda (x, y): x >= 0 and x < 4 and y >= 0 and y < 6, neighbors)
          neighbors = map(lambda (x, y): 4 * y + x, neighbors)
          #print 4 * y + x
          #print neighbors
          num = 0
          for n in neighbors:
            num |= 1 << n
          print '0x{:08x},'.format(num)

def indexes():
  for i in xrange(24):
    print '0x{:08x},'.format(1 << i)

indexes()
