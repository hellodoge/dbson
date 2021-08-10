## DBson
DBson is an in-memory data structures server that stores JSON-like objects.

### Example
For this example, I'll be using [a python driver](https://github.com/hellodoge/dbson_driver).\
Let's suppose we have to store information about some people.

``` python
In [1]: import dbson

In [2]: db = dbson.connect('localhost', 18889)
```

Using the `set` command, we can write objects to the database.

``` python
In [3]: db.execute({
   ...:     'command':    'set',
   ...:     'collection': 'people',
   ...:     'object':     'Maria',
   ...:     'data': {
   ...:         'age':    23,
   ...:         'family': {
   ...:             'husband':    'Alexey',
   ...:             'parents': ['Richard', 'Natalia']
   ...:         }
   ...:     }
   ...: })
Out[3]: {'success': True}
```

Database returned `True` for a `success` field. \
That means that the command was executed successfully.

For simplicity person's first name is used as the name of the object. \
You should not do that in real-world applications because the object name in the collection must be unique. \
A collection is a group of objects. In our example, we group all the people in one collection named, surprisingly, `people`.

Let's suppose Maria's husband is five years older. \
You can easily group commands like below. \
In this example, the `sum` command is used to add the age of Maria and `5`. \
A `selector` is a path to a field we want to work with inside an object.

``` python
In [4]: db.execute({
   ...:     'command':    'set',
   ...:     'collection': 'people',
   ...:     'object': {
   ...:         'command':    'get',
   ...:         'collection': 'people',
   ...:         'object':     'Maria',
   ...:         'selector':   'family:husband'
   ...:     },
   ...:     'selector':  'age',
   ...:     'result_of': {
   ...:         'command':   'sum',
   ...:         'elements':  [5],
   ...:         'result_of': {
   ...:             'command':    'get',
   ...:             'collection': 'people',
   ...:             'object':     'Maria',
   ...:             'selector':   'age'
   ...:         }
   ...:     }
   ...: })
Out[4]: {'success': True}
```

Note that we did not use Maria's husband's name in the example above.

Let's retrieve Alexey and see what we've got. \
`get` in the example below is a driver's utility method, that constructs you an actual command to a database and executes it.

``` python
In [5]: db.get(collection_name='people', object_name='Alexey')
Out[5]: {'data': {'age': 28}, 'success': True}
```

Information about the result of command execution is stored in the `data` field.

Let's add Maria's brother Kanye to a database. \
They sure have the same parents, so let's `set` this information.

``` python
In [6]: from dbson.commands import construct_get

In [7]: db.set(collection_name='people', object_name='Kanye', selector='family:parents',
   ...:        result_of = construct_get(collection_name='people',
   ...:                                  object_name=    'Maria',
   ...:                                  selector=       'family:parents'))
Out[7]: {'success': True}

In [8]: db.get(collection_name='people', object_name='Kanye')
Out[8]: {'data': {'family': {'parents': ['Richard', 'Natalia']}}, 'success': True}
```

### Binary format

DBson transfers data using its own binary format. \
I decided not to serialize data to bson, but the name stuck.

``` python
In [9]: from dbson.serializer import serialize

In [10]: with open('serialized.bin', 'wb') as f:
    ...:     serialize({
    ...:         'name':    'john',
    ...:         'surname': 'doe',
    ...:         'age':     32
    ...:     }, f)
    ...:

In [11]: !hexdump -C serialized.bin
00000000  6f 01 03 73 01 03 61 67  65 69 00 00 00 20 73 01  |o..s..agei... s.|
00000010  04 6e 61 6d 65 73 01 04  6a 6f 68 6e 73 01 07 73  |.names..johns..s|
00000020  75 72 6e 61 6d 65 73 01  03 64 6f 65              |urnames..doe|
0000002c
```

### TODO
Only basic functionality is done, and there is plenty of work to do.
- [ ] Multiple commands using one connection
- [ ] Authentication
- [ ] Commands working with groups of objects
- [ ] Persistence
