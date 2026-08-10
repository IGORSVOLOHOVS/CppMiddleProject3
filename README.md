# cpp-middle-project-sprint-3 <!-- omit in toc -->

- [Начало работы](#начало-работы)
- [Сборка проекта и запуск тестов](#сборка-проекта-и-запуск-тестов)
  - [Команды для сборки проекта](#команды-для-сборки-проекта)
  - [Команды для запуска приложения](#команды-для-запуска-приложения)
  - [Команда для запуска тестов](#команда-для-запуска-тестов)
  - [Команда для запуска clang-format - Обязательное требование перед сдачей работы на ревью](#команда-для-запуска-clang-format---обязательное-требование-перед-сдачей-работы-на-ревью)
  - [Команды для запуска отладчика](#команды-для-запуска-отладчика)
- [Сборка под Windows](#сборка-под-windows)
  - [Что нужно установить](#что-нужно-установить)
  - [Как собрать](#как-собрать)
  - [Пресеты CMake](#пресеты-cmake)


Шаблон репозитория для практического задания 3-го спринта «Мидл разработчик С++»

## Начало работы

1. Нажмите зелёную кнопку `Use this template`, затем `Create a new repository`.
2. Назовите свой репозиторий.
3. Склонируйте созданный репозиторий командой `git clone your-repository-name`.
4. Создайте новую ветку командой `git switch -c development`.
5. Откройте проект в `Visual Studio Code`.
6. Нажмите `F1` и откройте проект в dev-контейнере командой `Dev Containers: Reopen in Container`.

## Сборка проекта и запуск тестов

Данный репозиторий использует три инструмента:

- **Conan** — свободный менеджер пакетов для C и C++ с открытым исходным кодом (MIT). Позволяет настраивать процесс сборки программ, скачивать и устанавливать сторонние зависимости и необходимые инструменты. Подробнее о Conan:
  - https://habr.com/ru/articles/884464
  - https://docs.conan.io/2.0/tutorial/consuming_packages/build_simple_cmake_project.html
  - https://docs.conan.io/2.0/tutorial/consuming_packages/the_flexibility_of_conanfile_py.html

- **cmake** — генератор систем сборки для C и C++. Позволяет создавать проекты, которые могут компилироваться на различных платформах и с различными компиляторами. Подробнее о cmake:
  - https://dzen.ru/a/ZzZGUm-4o0u-IQlb
  - https://neerc.ifmo.ru/wiki/index.php?title=CMake_Tutorial
  - https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html

- **VS Code Dev Docker container** - Docker контейнер, который содержит полностью настроенное окружение для выполнение задания. Подробнее об этой функциональности:
  - https://habr.com/ru/articles/822707/ - "Почти все, что вы хотели бы знать про Docker"
  - https://code.visualstudio.com/docs/devcontainers/containers - официальная документация VS Code
  - https://www.youtube.com/watch?v=p9L7YFqHGk4 - "Docker container for VS Code"
  - https://www.youtube.com/watch?v=pg19Z8LL06w&t=174s&pp=ygUPRG9ja2VyY29udGFpbmVy - "Docker in 1 hour"

### Команды для сборки проекта

Используйте `F5` для выполнения следующих шагов:
- Создание папки `build`
- Вызов `conan` команд для установки требуемых библиотек и запуска процесса сборки
- Запуска `lldb` отладчика

### Команды для запуска приложения

```bash
cd build
./BookDB 
```

### Команда для запуска тестов

```bash
cd build
./BookDB_tests
```

### Команда для запуска clang-format - Обязательное требование перед сдачей работы на ревью

```bash
./run_clang_format.sh
```

### Команды для запуска отладчика

В `Visual Studio Code` настройки параметров для запуска отладчика находятся в `.vscode/launch.json` файле. Поскольку в этом файле уже есть одна конфигурация `Launch CryptoGuard` для запуска приложения, которое вычисляет контрольную сумму файла, то для запуска отладчика достаточно нажать `F5` или открыть окно `Run and Debug` комбинацией клавиш `Ctrl+Shift+D`.

## Сборка под Windows

Dev-контейнер остаётся основным способом сборки, но проект собирается и нативно —
компилятором MSVC, без Docker и без WSL. Точка входа одна: `scripts\build_windows.ps1`.

### Что нужно установить

- **Visual Studio 2022** (Community достаточно) с рабочей нагрузкой
  «Разработка классических приложений на C++». Нужен компилятор MSVC v143;
  проверялось на `cl.exe` 19.44. Более ранние версии могут не потянуть C++23:
  проект опирается на `std::print`, `std::formatter` и `std::flat_map`.
- **CMake ≥ 3.30** и **Ninja** на `PATH`. Оба приезжают вместе с компонентом
  Visual Studio «C++ CMake tools for Windows», если ставить их отдельно не хочется.
- **Conan 2** (`pip install conan`): из него приезжает GTest. Профиль `default`
  скрипт создаёт сам (`conan profile detect`), если его ещё нет.

Отдельно запускать «Developer Command Prompt» не требуется — скрипт сам находит
`vcvars64.bat` (через `vswhere`, с запасными путями) и вносит окружение MSVC в
свой процесс.

### Как собрать

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build_windows.ps1
```

Скрипт ставит зависимости через Conan, настраивает CMake, собирает всё и прогоняет
`ctest`, а в конце печатает пути к получившимся `.exe`. Результат лежит в
`build\windows`, основной исполняемый файл — `build\windows\BookDB.exe`,
тесты — `build\windows\BookDB_tests.exe`.

Полезные ключи:

| Ключ | Зачем |
| --- | --- |
| `-BuildType Debug` | сборка с отладочной информацией в `build\windows-debug` |
| `-Clean` | удалить каталог сборки и собрать с нуля |
| `-SkipTests` | не запускать `ctest` |

Три места, где код пришлось развести по компиляторам (на Linux ничего не изменилось):

- `std::flat_map` MSVC STL пока не поставляет вовсе, поэтому появился
  `include/flat_map_compat.hpp`: там, где `<flat_map>` есть, `bookdb::flat_map` —
  просто алиас на `std::flat_map`, а где нет — компактная замена на
  отсортированном `std::vector`.
- `CMAKE_CXX_STANDARD` для `cl.exe` понижен с 26 до 23: отдельного `/std:c++26`
  у MSVC нет, а 23 CMake разворачивает в `/std:c++latest` — тот же самый режим.
- Добавлен `/utf-8`: без него `cl.exe` читает исходники в системной ANSI-кодировке,
  и литералы вроде `Brontë` начинают зависеть от локали машины.

### Пресеты CMake

`CMakePresets.json` описывает обе платформы, поэтому IDE (Visual Studio, VS Code,
CLion) подхватывает конфигурацию сама:

| Пресет | Платформа | Генератор | Каталог сборки |
| --- | --- | --- | --- |
| `linux-default` | Linux / dev-контейнер | Unix Makefiles | `build/` |
| `windows-msvc-release` | Windows, MSVC x64 | Ninja | `build/windows/` |
| `windows-msvc-debug` | Windows, MSVC x64 | Ninja | `build/windows-debug/` |

Все пресеты ждут `conan_toolchain.cmake` в своём каталоге сборки, то есть
`conan install` должен отработать до `cmake --preset`. Windows-пресеты вдобавок
рассчитаны на окружение MSVC: если запускать их руками, а не через
`scripts\build_windows.ps1`, делать это надо из «Developer PowerShell for VS 2022»,
иначе CMake не найдёт `cl.exe`.

## Дополнительно

- Автодополнение `Ctrl + Space`. Для настройки автодополнения вам необходимо нажать `F1` и выполнить команду `clangd: Download language server`. VS Code сам предложит установить подходящую версию `clangd` (всплывашка в правом нижнем углу). После завершения установки потребуется перезагрузить окно (кнопка перезапуска будет находиться также справа снизу или нажать `F1` и выполнить команду `Developer: Reload Window`)

Если всё сделано правильно - после успешной сборки проекта вы сможете использовать автодополнение
