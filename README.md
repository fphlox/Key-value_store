# Key-value_store
Реализация хранилища данных типа ключ-значение с помощью двух самописных библиотек - хеш-таблицы и бинарного самобалансирующегося дерева поиска

Компиляция - make/gmake

Запуск тестов - make/gmake .test

Запуск программы с хеш-таблицей ./bin/release/cli/cli
Запуск программы с бинарным деревом ./bin/release/cli/cli btree

### Описание реализуемых функций key-value хранилища

### SET

Команда используется для установки ключа и его значения. В примере ниже ключом является строка `foo`, значением же
является структура, описанная выше. Значение полей новой записи вводится в порядке их описания в структуре. В качестве
необязательных параметров используется `EX` для указания срока жизни создаваемой записи. Если необязательное поле не
указано, то по умолчанию время жизни записи никак не ограничено.

Описание параметров команды `SET`:

```
SET <ключ> <Фамилия> <Имя> <Год рождения> <Город> <Число текущих коинов> EX <время в секундах>
```

Пример использования команды `SET` для создания записи без ограничения на время её существования:

```
SET foo Васильев Иван 2000 Москва 55 
> OK
SET foo Васильев 123 aaaaa Москва 55 
> ERROR: unable to cast value "aaaa" to type int 
```

Пример использования команды `SET` для создания записи с ограничением на время её существования. В этом примере запись
будет существовать на протяжении 10 секунд, после чего автоматически удалится:

```
SET foo Васильев Иван 2000 Москва 55 EX 10 
> OK
```

### GET

Команда используется для получения значения, связанного с ключом. Если такой записи нет, то будет возвращён `(null)`:

```
GET foo
> Васильев Иван 2000  Москва   55 
GET unknownkey
> (null)
```

### EXISTS

Эта команда проверяет, существует ли запись с данным ключом. Она возвращает `true` если объект существует или `false`
если нет:

```
EXISTS foo
> true
```

### DEL

Команда удаляет ключ и соответствующее значение, после чего возвращает `true`, если запись успешно удалена, в противном
случае - `false`:

```
DEL foo
> true
```

### UPDATE

Команда обновляет значение по соответствующему ключу, если такой ключ существует:

```
SET foo Вас И 20 Мос 5 
> OK
UPDATE foo Васильев Иван 2000 Москва 55
> OK

GET foo
> Васильев Иван 2000 Москва 55 
```

Если же какое-то поле менять не планируется, то на его месте ставится прочерк "-":

```
SET foo Вас И 20 Мос 5 
> OK
UPDATE foo Васильев - - - 55
> OK

GET foo
> Васильев И 20 Мос 55 
```

### KEYS

Возвращает все ключи, которые есть в хранилище:

```
KEYS
1) boo
2) foo
3) bar
```

### RENAME

Команда используется для переименования ключей:

```
RENAME foo foo2
> OK

GET foo
> (null)

GET foo2
> Васильев И 20 Мос 55 
```

### TTL

Когда ключ установлен с истечением срока действия, эту команду можно использовать для просмотра оставшегося времени.
Если записи с заданным ключом не существует, то возвращается `(null)`:

```
SET Васильев Иван 2000 Москва 55 EX 10
> OK
TTL foo
> 6
TTL foo
> 5
TTL foo
> 4
TTL foo
> (null)
```

### FIND

Эта команда используется для восстановления ключа (или ключей) по заданному значению. Аналогично команде `UPDATE`
необязательно указывать все значения из структуры студентов Школы 21. Если по каким-либо полям не будет выполняться
поиск, то на их месте просто проставляются прочерк `-`.

Пример использования команды `FIND` с поиском по всем полям структуры студентов:

```
FIND Васильев Иван 2000 Москва 55
> 1) foo
FIND Васильев Антон 1997 Тверь 55
> 1) boo
```

Пример использования команды `FIND` с поиском по фамилии и числу коинов:

```
FIND Васильев - - - 55
> 1) foo
> 2) boo
```

### SHOWALL

Команда для получения всех записей, которые содержатся в key-value хранилище на текущий момент:

```
SHOWALL
> № | Фамилия |   Имя   | Год |  Город   | Количество коинов |
> 1 "Васильев"  "Иван"    2000  "Москва"          55 
> 2  "Иванов"  "Василий"  2000  "Москва"          55 
```

### UPLOAD

Данная команда используется для загрузки данных из файла. Файл содержит список загружаемых данных в формате:

```
key1 "Васильев" "Иван" 2001 "Ростов" 55
key2 "Иванов" "Василий" 2000 "Москва" 55 
...
key101 "Сидоров" "Сергей" 1847 "Суздаль" 12312313 
```

Вызов команды:

```
UPLOAD ~/Desktop/TestData/file.dat
> OK 101
```

После ответа `OK` выводится число загруженных строк из файла.

### EXPORT

Данная команда используется для выгрузки данных, которые находятся в текущий момент в key-value хранилище в файл. Файл
должен на выходе содержать список данных в формате:

```
key1 "Васильев" "Иван" 2001 "Ростов" 55
key2 "Иванов" "Василий" 2000 "Москва" 55 
...
key101 "Сидоров" "Сергей" 1847 "Суздаль" 12312313 
```

Вызов команды:

```
EXPORT ~/Desktop/TestData/export.dat
> OK 101
```

После ответа `OK` выводится число выгруженных строк из файла.

### EXIT

Завершение программы