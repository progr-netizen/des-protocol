# DES Protocol (ECB, single-block) — scaffold

Цель: репозиторий, который «ведёт» ИИ-агента к корректной реализации DES.
Тесты заведомо падают, пока не будут заполнены таблицы и функции (см. `TODO(Agent)`).

## Как запускать
- **Локально:** `cmake -S . -B build && cmake --build build -j && ./build/tests/des_tests`
- **Replit:** просто нажми Run
- **GitHub Actions:** автозапуск при push/PR

## Что должен сделать ИИ-агент
- Заполнить таблицы в `src/tables.h` (IP, FP, E, P, PC1, PC2, SHIFTS, S-boxes)
- Реализовать:
  - `detail::ip`, `detail::fp`
  - `detail::key_schedule` (PC1 → сдвиги → PC2)
  - `detail::feistel` (E → XOR → 8×S → P)
- Добиться прохождения теста NIST-вектора:
  - K=133457799BBCDFF1
  - P=0123456789ABCDEF
  - C=85E813540F0AB405

## Подсказки для Copilot / ChatGPT
- **Prompt №1 (таблицы):**  
  “Вставь корректные таблицы DES (IP, FP, E, P, PC1, PC2, SHIFTS и S-box S1..S8) в `src/tables.h`, в формате 1-индексных массивов, как в стандарте.”
- **Prompt №2 (Feistel):**  
  “Реализуй `feistel`: расширение 32→48 через E, XOR с roundKey (48 бит), разрез на 8×6 бит, выборка строк/столбцов в S-box, сборка 32 бит и перестановка P.”
- **Prompt №3 (Key schedule):**  
  “Реализуй PC1 (64→56), циклические сдвиги C,D по `SHIFTS`, PC2 (56→48). Ключ раунда — младшие 48 бит `u64`.”
- **Prompt №4 (IP/FP):**  
  “Реализуй `ip`/`fp` с использованием общей `permute`.”

## Дальше
- Добавить режимы (ECB многоблочный, CBC).
- Проверить avalanche-эффект на рандомных парах.
- Сравнить скорость с OpenSSL (опционально).
