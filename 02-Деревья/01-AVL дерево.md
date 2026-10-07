## Глава 1. Основы AVL-дерева

### 1.1 История

В 1962 году два советских математика — **Георгий Максимович Адельсон-Вельский** и **Евгений Михайлович Ландис** — опубликовали статью, в которой предложили первое в истории самобалансирующееся бинарное дерево поиска. Дерево получило название **AVL** по первым буквам их фамилий.

Это была революционная идея: дерево, которое **автоматически** поддерживает свою высоту близкой к log₂(n), независимо от того, в каком порядке приходят данные.

### 1.2 Определение

> **AVL-дерево** — это бинарное дерево поиска, в котором для **каждого** узла выполняется инвариант:
>
> **|height(left) − height(right)| ≤ 1**

Разницу высот левого и правого поддеревьев узла называют **балансом** (или balance factor):

```
balance(v) = height(left(v)) − height(right(v))
```

В корректном AVL-дереве баланс каждого узла может принимать только три значения: **−1, 0, +1**.

- `balance = +1` → левое поддерево на 1 выше (левое тяжелее)
- `balance = 0` → поддеревья равной высоты
- `balance = −1` → правое поддерево на 1 выше (правое тяжелее)

Если после вставки или удаления баланс какого-то узла стал **+2 или −2**, дерево нужно **починить** с помощью поворотов.

### 1.3 Высота AVL-дерева: доказательство O(log n)

Главный вопрос: **какова максимальная высота** AVL-дерева с n узлами?

Подойдём с другой стороны: какое **минимальное число узлов** N(h) нужно, чтобы построить AVL-дерево высоты h?

Чтобы дерево имело высоту h и при этом было «минимальным» (с наименьшим числом узлов), одно поддерево должно иметь высоту h−1, а другое — h−2 (максимальный допустимый перекос):

```
N(h) = N(h−1) + N(h−2) + 1
```

Базовые случаи:
- N(0) = 1 (один узел)
- N(1) = 2 (корень + один ребёнок)

Посчитаем:

| h | N(h) | Комментарий |
|---|------|-------------|
| 0 | 1 | |
| 1 | 2 | |
| 2 | 4 | = 2 + 1 + 1 |
| 3 | 7 | = 4 + 2 + 1 |
| 4 | 12 | = 7 + 4 + 1 |
| 5 | 20 | = 12 + 7 + 1 |

Заметь: N(h) = F(h+3) − 1, где F — числа Фибоначчи!

Поскольку числа Фибоначчи растут экспоненциально (как φʰ, где φ ≈ 1.618), получаем:

```
N(h) ≈ φʰ
n ≥ φʰ
h ≤ logφ(n) ≈ 1.44 · log₂(n)
```

> **Высота AVL-дерева не превышает 1.44 · log₂(n)**

Это всего на 44% больше, чем у идеально сбалансированного дерева. На практике разница ещё меньше.

### 1.4 Структура узла

```cpp
struct Node {
    int key;           // ключ
    int height;        // высота поддерева с корнем в этом узле
    Node* left;        // левый ребёнок
    Node* right;       // правый ребёнок
};
```

Вспомогательные функции:

```cpp
int height(Node* v) {
    return v ? v->height : 0;
}

int balance(Node* v) {
    return height(v->left) - height(v->right);
}

void fix_height(Node* v) {
    v->height = 1 + max(height(v->left), height(v->right));
}
```

---

## Глава 2. Повороты

Повороты — это **локальные перестройки** дерева, которые:
- Выполняются за **O(1)** (перенаправление нескольких указателей)
- **Сохраняют** свойство BST (порядок ключей)
- **Изменяют** высоты поддеревьев, устраняя дисбаланс

### 2.1 Когда возникает дисбаланс?

После вставки или удаления баланс некоторого узла может стать **+2** или **−2**. Есть четыре случая:

| Баланс узла | Баланс ребёнка | Случай      | Название                    |
| ----------- | -------------- | ----------- | --------------------------- |
| +2          | +1 или 0       | Left-Left   | Малый правый поворот        |
| +2          | −1             | Left-Right  | Большой лево-правый поворот |
| −2          | −1 или 0       | Right-Right | Малый левый поворот         |
| −2          | +1             | Right-Left  | Большой право-левый поворот |
### 2.2 Малый правый поворот (Right Rotation)

**Когда:** баланс узла = +2, баланс левого ребёнка = +1 или 0 (Left-Left случай)

```
        y (+2)              x (0 или +1)
       / \                /   \
    x (+1) C    →       A      y
   / \                        / \
  A   B                     B   C
```

Код:

```cpp
Node* rotate_right(Node* y) {
    Node* x = y->left;
    y->left = x->right;    // B переходит к y
    x->right = y;           // y становится правым ребёнком x
    fix_height(y);          // сначала y (он теперь ниже)
    fix_height(x);          // потом x
    return x;               // x — новый корень
}
```
![[Pasted image 20260531204058.png]]
### 2.3 Малый левый поворот (Left Rotation)

**Когда:** баланс узла = −2, баланс правого ребёнка = −1 или 0 (Right-Right случай)

Полностью симметричен правому:

```
    x (−2)                y
   / \                  /   \
  A   y (−1)    →     x      C
     / \             / \
    B   C           A   B
```

Код:

```cpp
Node* rotate_left(Node* x) {
    Node* y = x->right;
    x->right = y->left;    // B переходит к x
    y->left = x;            // x становится левым ребёнком y
    fix_height(x);
    fix_height(y);
    return y;
}
```
![[Pasted image 20260531203951.png]]
### 2.4 Большой лево-правый поворот (Left-Right)

**Когда:** баланс узла = +2, баланс левого ребёнка = −1

Один правый поворот не поможет! Нужно сначала повернуть левого ребёнка влево, потом корень вправо:

```
      z (+2)           z (+2)             y
     / \              / \               /   \
   x   D   →       y   D    →        x       z
  / \              / \               / \     / \
 A   y            x   C            A   B   C   D
    / \          / \
   B   C        A   B
```

Код:

```cpp
// Встроено в функцию balance() ниже:
// 1. rotate_left(z->left)
// 2. rotate_right(z)
```
![[Pasted image 20260531204405.png]]
### 2.5 Большой право-левый поворот (Right-Left)

**Когда:** баланс узла = −2, баланс правого ребёнка = +1

Симметрично предыдущему: сначала правый поворот правого ребёнка, потом левый поворот корня.
![[Pasted image 20260531204002.png]]
### 2.6 Единая функция балансировки

Все четыре случая можно объединить в одну функцию:

```cpp
Node* do_balance(Node* v) {
    fix_height(v);

    if (balance(v) == +2) {
        // Left-heavy
        if (balance(v->left) < 0) {
            // Left-Right случай: сначала левый поворот левого ребёнка
            v->left = rotate_left(v->left);
        }
        return rotate_right(v);
    }

    if (balance(v) == -2) {
        // Right-heavy
        if (balance(v->right) > 0) {
            // Right-Left случай: сначала правый поворот правого ребёнка
            v->right = rotate_right(v->right);
        }
        return rotate_left(v);
    }

    return v; // балансировка не нужна
}
```


---

## Глава 3. Операции над AVL-деревом

### 3.1 Поиск

Поиск в AVL-дереве — это обычный поиск в BST. Баланс не нарушается, повороты не нужны:

```cpp
Node* search(Node* v, int key) {
    if (!v) return nullptr;           // не нашли
    if (key == v->key) return v;      // нашли
    if (key < v->key) return search(v->left, key);
    return search(v->right, key);
}
```

**Сложность:** O(h) = O(log n), потому что высота AVL-дерева гарантированно O(log n).

### 3.2 Вставка

Вставка состоит из двух фаз:
1. **Спуск** — находим место для нового узла (как в обычном BST)
2. **Подъём** — при возврате из рекурсии обновляем высоту и балансируем каждый узел на пути

```cpp
Node* insert(Node* v, int key) {
    // 1. Обычная вставка BST
    if (!v) return new Node(key);

    if (key < v->key)
        v->left = insert(v->left, key);
    else if (key > v->key)
        v->right = insert(v->right, key);
    else
        return v;  // ключ уже существует

    // 2. Балансировка при подъёме
    return do_balance(v);
}
```

**Важный факт:** при вставке достаточно **максимум 2 поворотов** (один одинарный или один двойной). После первого поворота высота поддерева восстанавливается, и дальнейшие узлы на пути вверх не нуждаются в балансировке.

#### Пошаговый пример

Вставим последовательно: **5, 3, 7, 2, 4, 1**

```
Шаг 1: вставка 5          5 (balance=0)

Шаг 2: вставка 3          5 (balance=+1)
                          /
                         3

Шаг 3: вставка 7          5 (balance=0)
                          / \
                         3   7

Шаг 4: вставка 2          5 (balance=+1)
                          / \
                         3   7
                        /
                       2

Шаг 5: вставка 4          5 (balance=+1)
                          / \
                         3   7
                        / \
                       2   4

Шаг 6: вставка 1          5 (balance=+2!) ← дисбаланс!
                          / \
                         3   7
                        / \
                       2   4
                      /
                     1

Баланс узла 5 = +2, баланс узла 3 = +1 → Left-Left → правый поворот:

                         3 (balance=0)
                        / \
                       2   5
                      /   / \
                     1   4   7
```

### 3.3 Удаление

Удаление сложнее вставки. Три случая для удаляемого узла:

1. **Лист** (нет детей) → просто удаляем
2. **Один ребёнок** → заменяем узел его ребёнком
3. **Два ребёнка** → находим **преемника** (минимум в правом поддереве) или **предшественника** (максимум в левом), копируем его ключ в удаляемый узел, удаляем преемника рекурсивно

После удаления — балансируем на пути вверх:

```cpp
Node* find_min(Node* v) {
    return v->left ? find_min(v->left) : v;
}

Node* remove_min(Node* v) {
    if (!v->left) return v->right;
    v->left = remove_min(v->left);
    return do_balance(v);
}

Node* remove(Node* v, int key) {
    if (!v) return nullptr;

    if (key < v->key) {
        v->left = remove(v->left, key);
    } else if (key > v->key) {
        v->right = remove(v->right, key);
    } else {
        // Нашли узел для удаления
        Node* left = v->left;
        Node* right = v->right;
        delete v;

        if (!right) return left;  // случаи 1 и 2

        // Случай 3: заменяем минимумом правого поддерева
        Node* min_node = find_min(right);
        min_node->right = remove_min(right);
        min_node->left = left;
        return do_balance(min_node);
    }

    return do_balance(v);
}
```

**Важный факт:** при удалении может потребоваться **O(log n) поворотов** — в отличие от вставки, где достаточно двух. Это потому, что после поворота на одном уровне высота поддерева может уменьшиться, вызывая дисбаланс выше.

### 3.4 Нахождение минимума, максимума, следующего и предыдущего

```cpp
// Минимум: идём влево до упора
Node* min(Node* v) {
    return v->left ? min(v->left) : v;
}

// Максимум: идём вправо до упора
Node* max(Node* v) {
    return v->right ? max(v->right) : v;
}
```

Все эти операции — O(h) = O(log n).

---

## Глава 4. Анализ и практика

### 4.1 Сводка сложностей

| Операция | Сложность | Поворотов |
|----------|-----------|-----------|
| Поиск | O(log n) | 0 |
| Вставка | O(log n) | ≤ 2 |
| Удаление | O(log n) | ≤ O(log n) |
| Минимум / Максимум | O(log n) | 0 |

### 4.2 AVL vs обычный BST

| | Обычный BST | AVL |
|---|---|---|
| Лучший случай | O(log n) | O(log n) |
| Худший случай | **O(n)** 😱 | **O(log n)** ✅ |
| Доп. память на узел | 0 | 1 int (высота) |
| Сложность реализации | Простая | Средняя |

### 4.3 Полная реализация на C++

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

int height(Node* v) {
    return v ? v->height : 0;
}

int balance_factor(Node* v) {
    return height(v->left) - height(v->right);
}

void fix_height(Node* v) {
    v->height = 1 + max(height(v->left), height(v->right));
}

Node* rotate_right(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    fix_height(y);
    fix_height(x);
    return x;
}

Node* rotate_left(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    fix_height(x);
    fix_height(y);
    return y;
}

Node* do_balance(Node* v) {
    fix_height(v);

    if (balance_factor(v) == 2) {
        if (balance_factor(v->left) < 0)
            v->left = rotate_left(v->left);
        return rotate_right(v);
    }

    if (balance_factor(v) == -2) {
        if (balance_factor(v->right) > 0)
            v->right = rotate_right(v->right);
        return rotate_left(v);
    }

    return v;
}

Node* insert(Node* v, int key) {
    if (!v) return new Node(key);
    if (key < v->key)
        v->left = insert(v->left, key);
    else if (key > v->key)
        v->right = insert(v->right, key);
    else
        return v;
    return do_balance(v);
}

Node* find_min(Node* v) {
    return v->left ? find_min(v->left) : v;
}

Node* remove_min(Node* v) {
    if (!v->left) return v->right;
    v->left = remove_min(v->left);
    return do_balance(v);
}

Node* remove(Node* v, int key) {
    if (!v) return nullptr;
    if (key < v->key) {
        v->left = remove(v->left, key);
    } else if (key > v->key) {
        v->right = remove(v->right, key);
    } else {
        Node* l = v->left;
        Node* r = v->right;
        delete v;
        if (!r) return l;
        Node* min_node = find_min(r);
        min_node->right = remove_min(r);
        min_node->left = l;
        return do_balance(min_node);
    }
    return do_balance(v);
}

Node* search(Node* v, int key) {
    if (!v) return nullptr;
    if (key == v->key) return v;
    if (key < v->key) return search(v->left, key);
    return search(v->right, key);
}

// Вспомогательная функция для вывода дерева
void print_tree(Node* v, int indent = 0) {
    if (!v) return;
    print_tree(v->right, indent + 4);
    for (int i = 0; i < indent; i++) cout << ' ';
    cout << v->key << " (h=" << v->height
         << ", b=" << balance_factor(v) << ")" << endl;
    print_tree(v->left, indent + 4);
}

int main() {
    Node* root = nullptr;

    // Вставляем отсортированные данные — AVL справится!
    for (int i = 1; i <= 15; i++) {
        root = insert(root, i);
    }

    cout << "After inserting 1..15:" << endl;
    print_tree(root);
    cout << "Height: " << height(root) << endl;

    // Удаляем несколько элементов
    root = remove(root, 8);
    root = remove(root, 4);
    root = remove(root, 12);

    cout << "\nAfter removing 8, 4, 12:" << endl;
    print_tree(root);
    cout << "Height: " << height(root) << endl;

    return 0;
}
```

### 4.4 Типичные ошибки при реализации

1. **Забыть вызвать `fix_height`** после поворота — высота будет неверной, последующие балансировки сломаются

2. **Неправильный порядок `fix_height`** — в повороте сначала обновляем высоту узла, который стал ниже (ребёнком), потом того, что стал выше (родителем)

3. **Не обновлять корень** — `insert` и `remove` возвращают новый корень поддерева; если не присвоить результат, дерево «рассыпается»:
   ```cpp
   // НЕПРАВИЛЬНО:
   insert(root, 5);

   // ПРАВИЛЬНО:
   root = insert(root, 5);
   ```

4. **Утечки памяти** — при удалении узла не забывать `delete`. В учебных целях можно использовать умные указатели или вообще не удалять (для контестов)

5. **Не обрабатывать случай двух детей при удалении** — нужно найти преемника, скопировать ключ и удалить преемника, а не сам узел
