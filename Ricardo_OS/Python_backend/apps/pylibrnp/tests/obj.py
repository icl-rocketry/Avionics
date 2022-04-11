import copy

class t():
    def __init__(self):
        self.a = 1
        self.b = 2
        self.c = 3

    
T = t()
mv = vars(T)
print(mv)
keys = vars(T).keys()
print(type(keys))
print(type(mv['a']))
a_copy = mv['a']
print(mv['a'])

# vals = [*mv.values()]
# print(vals)
# print(type(vals))

# print(id(vals[0]))
# print(id(T.a))

# vals[0] = copy.deepcopy(10)

# print(T.a)

