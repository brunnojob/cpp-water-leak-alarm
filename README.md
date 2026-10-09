# Water Leak Monitor

Análise de leituras com filtro, histerese, confirmação temporal, falha de sensor e alarme retido até reconhecimento.

## Executar

Requisitos: C++20 e CMake.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
build/leak_monitor leituras.csv > resultado.json
```

## Funcionamento

Entrada CSV: `timestamp_ms,adc`. A biblioteca expõe reconhecimento manual e validação de dados. Testes incluem alarme, recuperação, atraso e valores não finitos. A atuação física exige integração e validação do dispositivo.

## Persistência de resultados

O arquivo de operações está em [vercel-home-telemetry-api.vercel.app](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=cpp-water-leak-alarm). As migrações Supabase estão no [repositório da API](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue resultado.json --project cpp-water-leak-alarm
python cloud/sync.py sync
```

Defina `BRUNNODEV_ACCESS_TOKEN` com sua sessão. A fila SQLite conserva os relatórios até confirmação do servidor; o mesmo conteúdo não gera registros duplicados. Tokens não são gravados no código.
