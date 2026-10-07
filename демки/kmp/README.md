# KMP demo

Демка показывает КМП в рантайме с подробным логом: построение префикс-функции, поиск через `P#T`, классический потоковый КМП и вариант реального времени через автомат.

## Компиляция

```bash
make
```

## Запуск

```bash
./kmp_demo all abacaba aba
```

Аргументы:

```text
./kmp_demo <mode> <text> <pattern>
./kmp_demo pi <pattern>
./kmp_demo z2pi <pattern>
```

Режимы:

- `pi` — только построение префикс-функции для `pattern`;
- `z2pi` — построение `pi` на основе Z-функции для `pattern`;
- `concat` — КМП через строку `pattern + '#' + text`;
- `stream` — классический потоковый КМП без склейки строк;
- `automaton` — КМП реального времени через таблицу переходов;
- `all` — все режимы сразу.

Если аргументы не переданы, используется пример:

```text
text = abacaba
pattern = aba
```

## Быстрые команды

```bash
make run
./kmp_demo pi ababaca
./kmp_demo z2pi ababaca
./kmp_demo stream ababababa ababa
./kmp_demo automaton aaaaa aa
./kmp_demo concat mississippi issi
```

## Что смотреть в логе

- `q` или `state` — сколько символов образца уже совпало.
- `pi[k]` — куда откатываемся после несовпадения.
- `fallback` — момент, когда алгоритм не начинает с нуля, а переиспользует уже найденный бордер.
- `FOUND` — найденное вхождение образца в текст.
