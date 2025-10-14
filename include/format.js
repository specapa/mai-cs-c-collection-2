const fs = require("fs");

// Выравнивание
function alignBackslashes(lines) {
    let maxLen = 0;
    lines.forEach(line => {
        let match = line.match(/^(.*?)(\\)\s*$/);
        if (match) {
            maxLen = Math.max(maxLen, match[1].length);
        }
    });

    return lines.map(line => 
        line.replace(/^(.*?)(\\)\s*$/, (_, codePart, slash) => 
            codePart.padEnd(maxLen) + " " + slash
        )
    );
}

// Убрать лишние пробелы
function reduceSpacesBeforeBackslash(lines) {
    return lines.map(line => 
        line.replace(/^(.*?)\s*(\\)\s*$/, (_, codePart, slash) => 
            codePart + " " + slash
        )
    );
}

// Парсим аргументы командной строки
const args = process.argv.slice(2);
const reduceSpaces = args.includes("-r");
const fileIndex = args.indexOf("-f");

if (fileIndex === -1 || fileIndex === args.length - 1) {
    console.error("Ошибка: Укажите файлы после флага -f.\nИспользование: node format-macros.js [-r] -f <файл1> <файл2> ...");
    process.exit(1);
}

// Список файлов
const filePaths = args.slice(fileIndex + 1);

filePaths.forEach(filePath => {
    try {
        let code = fs.readFileSync(filePath, "utf8");
        let lines = code.split("\n");

        let formattedLines = reduceSpaces ? 
            reduceSpacesBeforeBackslash(lines) : 
            alignBackslashes(lines);

        fs.writeFileSync(filePath, formattedLines.join("\n"), "utf8");
        console.log(`Файл обработан: ${filePath}`);
    } catch (error) {
        console.error(`Ошибка при обработке ${filePath}: ${error.message}`);
    }
});