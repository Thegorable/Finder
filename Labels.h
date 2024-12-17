#pragma once
#include<string>
enum class Language { RU, EN, UK, PT_BR, PT_PT, AR, TR, EL, CS, HU, SV, FI, DA, NB, ID, TH, VI, NL, HI, ES, RO, DE, FR, IT, PL, JA, ZH, KO };

struct LabelsLocal_EN {
    const std::u16string analizing_label = u"Analizing...";
    const std::u16string start_instructions = u"Enter file name:";
    const std::u16string found_list_label = u"Matched files";
    const std::u16string no_found_list_label = u"No matched files found";
    const std::u16string w_max_path_length = u"Warning! The maximum path length was exceeded! That paths weren't use:\n\n";
    const std::u16string w_skipped_paths = u"Warning! The next paths are not exists and were skipped:\n\n";
};

struct LabelsLocal_RU {
    const std::u16string analizing_label = u"Анализ...";
    const std::u16string start_instructions = u"Введите имя файла:";
    const std::u16string found_list_label = u"Найденные файлы";
    const std::u16string no_found_list_label = u"Совпадений не найдено";
    const std::u16string w_max_path_length = u"Предупреждение! Превышена максимальная длина пути. Следующие пути не были использованы:\n\n";
    const std::u16string w_skipped_paths = u"Предупреждение! Следующие пути не существуют и были пропущены:\n\n";
};

struct LabelsLocal_UK {
    const std::u16string analizing_label = u"Аналіз...";
    const std::u16string start_instructions = u"Введіть ім'я файлу:";
    const std::u16string found_list_label = u"Знайдені файли";
    const std::u16string no_found_list_label = u"Співпадінь не знайдено";
    const std::u16string w_max_path_length = u"Попередження! Перевищена максимальна довжина шляху. Наступні шляхи не були використані:\n\n";
    const std::u16string w_skipped_paths = u"Попередження! Наступні шляхи не існують і були пропущені:\n\n";
};

struct LabelsLocal_PT_BR {
    const std::u16string analizing_label = u"Analisando...";
    const std::u16string start_instructions = u"Digite o nome do arquivo:";
    const std::u16string found_list_label = u"Arquivos encontrados";
    const std::u16string no_found_list_label = u"Nenhum arquivo encontrado";
    const std::u16string w_max_path_length = u"Aviso! O comprimento máximo do caminho foi excedido. Os seguintes caminhos não foram usados:\n\n";
    const std::u16string w_skipped_paths = u"Aviso! Os seguintes caminhos não existem e foram ignorados:\n\n";
};

struct LabelsLocal_PT_PT {
    const std::u16string analizing_label = u"A analisar...";
    const std::u16string start_instructions = u"Insira o nome do ficheiro:";
    const std::u16string found_list_label = u"Ficheiros encontrados";
    const std::u16string no_found_list_label = u"Nenhum ficheiro encontrado";
    const std::u16string w_max_path_length = u"Aviso! O comprimento máximo do caminho foi excedido. Os seguintes caminhos não foram usados:\n\n";
    const std::u16string w_skipped_paths = u"Aviso! Os seguintes caminhos não existem e foram ignorados:\n\n";
};

struct LabelsLocal_AR {
    const std::u16string analizing_label = u"جار التحليل...";
    const std::u16string start_instructions = u"أدخل اسم الملف:";
    const std::u16string found_list_label = u"الملفات المطابقة";
    const std::u16string no_found_list_label = u"لم يتم العثور على ملفات مطابقة";
    const std::u16string w_max_path_length = u"تحذير! تم تجاوز الحد الأقصى لطول المسار. لم تُستخدم المسارات التالية:\n\n";
    const std::u16string w_skipped_paths = u"تحذير! المسارات التالية غير موجودة وتم تخطيها:\n\n";
};

struct LabelsLocal_TR {
    const std::u16string analizing_label = u"Analiz ediliyor...";
    const std::u16string start_instructions = u"Dosya adını girin:";
    const std::u16string found_list_label = u"Eşleşen dosyalar";
    const std::u16string no_found_list_label = u"Eşleşen dosya bulunamadı";
    const std::u16string w_max_path_length = u"Uyarı! Maksimum yol uzunluğu aşıldı. Aşağıdaki yollar kullanılmadı:\n\n";
    const std::u16string w_skipped_paths = u"Uyarı! Aşağıdaki yollar mevcut değil ve atlandı:\n\n";
};

struct LabelsLocal_EL {
    const std::u16string analizing_label = u"Ανάλυση...";
    const std::u16string start_instructions = u"Εισαγάγετε το όνομα του αρχείου:";
    const std::u16string found_list_label = u"Αντιστοιχισμένα αρχεία";
    const std::u16string no_found_list_label = u"Δεν βρέθηκαν αρχεία που να ταιριάζουν";
    const std::u16string w_max_path_length = u"Προειδοποίηση! Το μέγιστο μήκος διαδρομής ξεπεράστηκε. Οι παρακάτω διαδρομές δεν χρησιμοποιήθηκαν:\n\n";
    const std::u16string w_skipped_paths = u"Προειδοποίηση! Οι παρακάτω διαδρομές δεν υπάρχουν και παραλείφθηκαν:\n\n";
};

struct LabelsLocal_CS {
    const std::u16string analizing_label = u"Analyzování...";
    const std::u16string start_instructions = u"Zadejte název souboru:";
    const std::u16string found_list_label = u"Nalezené soubory";
    const std::u16string no_found_list_label = u"Nenalezeny žádné odpovídající soubory";
    const std::u16string w_max_path_length = u"Varování! Překročena maximální délka cesty. Následující cesty nebyly použity:\n\n";
    const std::u16string w_skipped_paths = u"Varování! Následující cesty neexistují a byly přeskočeny:\n\n";
};

struct LabelsLocal_HU {
    const std::u16string analizing_label = u"Elemzés...";
    const std::u16string start_instructions = u"Adja meg a fájl nevét:";
    const std::u16string found_list_label = u"Talált fájlok";
    const std::u16string no_found_list_label = u"Nem találhatók megfelelő fájlok";
    const std::u16string w_max_path_length = u"Figyelmeztetés! Túllépte a maximális útvonalhosszt. Az alábbi útvonalak nem lettek használva:\n\n";
    const std::u16string w_skipped_paths = u"Figyelmeztetés! Az alábbi útvonalak nem léteznek, és kihagyásra kerültek:\n\n";
};

struct LabelsLocal_SV {
    const std::u16string analizing_label = u"Analyserar...";
    const std::u16string start_instructions = u"Ange filnamn:";
    const std::u16string found_list_label = u"Hittade filer";
    const std::u16string no_found_list_label = u"Inga matchande filer hittades";
    const std::u16string w_max_path_length = u"Varning! Den maximala sökvägslängden överskreds. Följande sökvägar användes inte:\n\n";
    const std::u16string w_skipped_paths = u"Varning! Följande sökvägar finns inte och hoppades över:\n\n";
};

struct LabelsLocal_FI {
    const std::u16string analizing_label = u"Analysoidaan...";
    const std::u16string start_instructions = u"Anna tiedoston nimi:";
    const std::u16string found_list_label = u"Löydetyt tiedostot";
    const std::u16string no_found_list_label = u"Ei löytynyt vastaavia tiedostoja";
    const std::u16string w_max_path_length = u"Varoitus! Enimmäispolun pituus ylitettiin. Seuraavia polkuja ei käytetty:\n\n";
    const std::u16string w_skipped_paths = u"Varoitus! Seuraavia polkuja ei ole olemassa ja ne ohitettiin:\n\n";
};

struct LabelsLocal_DA {
    const std::u16string analizing_label = u"Analyserer...";
    const std::u16string start_instructions = u"Indtast filnavn:";
    const std::u16string found_list_label = u"Fundne filer";
    const std::u16string no_found_list_label = u"Ingen matchende filer fundet";
    const std::u16string w_max_path_length = u"Advarsel! Den maksimale stivejslængde blev overskredet. Følgende stier blev ikke brugt:\n\n";
    const std::u16string w_skipped_paths = u"Advarsel! Følgende stier findes ikke og blev ignoreret:\n\n";
};

struct LabelsLocal_NB {
    const std::u16string analizing_label = u"Analyserer...";
    const std::u16string start_instructions = u"Oppgi filnavn:";
    const std::u16string found_list_label = u"Matchende filer";
    const std::u16string no_found_list_label = u"Ingen matchende filer funnet";
    const std::u16string w_max_path_length = u"Advarsel! Maksimal sti-lengde ble overskredet. Følgende stier ble ikke brukt:\n\n";
    const std::u16string w_skipped_paths = u"Advarsel! Følgende stier eksisterer ikke og ble hoppet over:\n\n";
};

struct LabelsLocal_ID {
    const std::u16string analizing_label = u"Menganalisis...";
    const std::u16string start_instructions = u"Masukkan nama file:";
    const std::u16string found_list_label = u"File yang cocok";
    const std::u16string no_found_list_label = u"Tidak ada file yang cocok ditemukan";
    const std::u16string w_max_path_length = u"Peringatan! Panjang jalur maksimum terlampaui. Jalur berikut tidak digunakan:\n\n";
    const std::u16string w_skipped_paths = u"Peringatan! Jalur berikut tidak ada dan dilewati:\n\n";
};

struct LabelsLocal_TH {
    const std::u16string analizing_label = u"กำลังวิเคราะห์...";
    const std::u16string start_instructions = u"กรอกชื่อไฟล์:";
    const std::u16string found_list_label = u"ไฟล์ที่ตรงกัน";
    const std::u16string no_found_list_label = u"ไม่พบไฟล์ที่ตรงกัน";
    const std::u16string w_max_path_length = u"คำเตือน! เกินความยาวเส้นทางสูงสุด เส้นทางต่อไปนี้ไม่ได้ถูกใช้:\n\n";
    const std::u16string w_skipped_paths = u"คำเตือน! เส้นทางต่อไปนี้ไม่มีอยู่จริงและถูกข้ามไป:\n\n";
};

struct LabelsLocal_VI {
    const std::u16string analizing_label = u"Đang phân tích...";
    const std::u16string start_instructions = u"Nhập tên tệp:";
    const std::u16string found_list_label = u"Tệp phù hợp";
    const std::u16string no_found_list_label = u"Không tìm thấy tệp phù hợp";
    const std::u16string w_max_path_length = u"Cảnh báo! Đã vượt quá độ dài đường dẫn tối đa. Các đường dẫn sau không được sử dụng:\n\n";
    const std::u16string w_skipped_paths = u"Cảnh báo! Các đường dẫn sau không tồn tại và đã bị bỏ qua:\n\n";
};

struct LabelsLocal_NL {
    const std::u16string analizing_label = u"Bezig met analyseren...";
    const std::u16string start_instructions = u"Voer bestandsnaam in:";
    const std::u16string found_list_label = u"Overeenkomende bestanden";
    const std::u16string no_found_list_label = u"Geen overeenkomende bestanden gevonden";
    const std::u16string w_max_path_length = u"Waarschuwing! De maximale padlengte is overschreden. De volgende paden werden niet gebruikt:\n\n";
    const std::u16string w_skipped_paths = u"Waarschuwing! De volgende paden bestaan niet en zijn overgeslagen:\n\n";
};

struct LabelsLocal_HI {
    const std::u16string analizing_label = u"विश्लेषण कर रहा है...";
    const std::u16string start_instructions = u"फ़ाइल नाम दर्ज करें:";
    const std::u16string found_list_label = u"मेल खाते फाइलें";
    const std::u16string no_found_list_label = u"कोई मेल खाती फाइलें नहीं मिलीं";
    const std::u16string w_max_path_length = u"चेतावनी! अधिकतम पथ लंबाई पार हो गई। निम्नलिखित पथों का उपयोग नहीं किया गया:\n\n";
    const std::u16string w_skipped_paths = u"चेतावनी! निम्नलिखित पथ मौजूद नहीं हैं और छोड़ दिए गए:\n\n";
};

struct LabelsLocal_ES {
    const std::u16string analizing_label = u"Analizando...";
    const std::u16string start_instructions = u"Ingrese el nombre del archivo:";
    const std::u16string found_list_label = u"Archivos coincidentes";
    const std::u16string no_found_list_label = u"No se encontraron archivos coincidentes";
    const std::u16string w_max_path_length = u"Advertencia! Se excedió la longitud máxima de la ruta. Las siguientes rutas no fueron utilizadas:\n\n";
    const std::u16string w_skipped_paths = u"Advertencia! Las siguientes rutas no existen y fueron omitidas:\n\n";
};

struct LabelsLocal_RO {
    const std::u16string analizing_label = u"Se analizează...";
    const std::u16string start_instructions = u"Introduceți numele fișierului:";
    const std::u16string found_list_label = u"Fișiere potrivite";
    const std::u16string no_found_list_label = u"Nu s-au găsit fișiere potrivite";
    const std::u16string w_max_path_length = u"Avertisment! Lungimea maximă a căii a fost depășită. Următoarele căi nu au fost utilizate:\n\n";
    const std::u16string w_skipped_paths = u"Avertisment! Următoarele căi nu există și au fost omise:\n\n";
};

struct LabelsLocal_DE {
    const std::u16string analizing_label = u"Analysiere...";
    const std::u16string start_instructions = u"Geben Sie den Dateinamen ein:";
    const std::u16string found_list_label = u"Gefundene Dateien";
    const std::u16string no_found_list_label = u"Keine passenden Dateien gefunden";
    const std::u16string w_max_path_length = u"Warnung! Die maximale Pfadlänge wurde überschritten. Die folgenden Pfade wurden nicht verwendet:\n\n";
    const std::u16string w_skipped_paths = u"Warnung! Die folgenden Pfade existieren nicht und wurden übersprungen:\n\n";
};

struct LabelsLocal_FR {
    const std::u16string analizing_label = u"Analyse en cours...";
    const std::u16string start_instructions = u"Entrez le nom du fichier :";
    const std::u16string found_list_label = u"Fichiers correspondants";
    const std::u16string no_found_list_label = u"Aucun fichier correspondant trouvé";
    const std::u16string w_max_path_length = u"Avertissement ! La longueur maximale du chemin a été dépassée. Les chemins suivants n'ont pas été utilisés :\n\n";
    const std::u16string w_skipped_paths = u"Avertissement ! Les chemins suivants n'existent pas et ont été ignorés :\n\n";
};

struct LabelsLocal_IT {
    const std::u16string analizing_label = u"Analisi in corso...";
    const std::u16string start_instructions = u"Inserisci il nome del file:";
    const std::u16string found_list_label = u"File corrispondenti";
    const std::u16string no_found_list_label = u"Nessun file corrispondente trovato";
    const std::u16string w_max_path_length = u"Avviso! Lunghezza massima del percorso superata. I seguenti percorsi non sono stati utilizzati:\n\n";
    const std::u16string w_skipped_paths = u"Avviso! I seguenti percorsi non esistono e sono stati ignorati:\n\n";
};

struct LabelsLocal_PL {
    const std::u16string analizing_label = u"Analizowanie...";
    const std::u16string start_instructions = u"Wpisz nazwę pliku:";
    const std::u16string found_list_label = u"Dopasowane pliki";
    const std::u16string no_found_list_label = u"Nie znaleziono dopasowanych plików";
    const std::u16string w_max_path_length = u"Ostrzeżenie! Przekroczono maksymalną długość ścieżki. Następujące ścieżki nie zostały użyte:\n\n";
    const std::u16string w_skipped_paths = u"Ostrzeżenie! Następujące ścieżki nie istnieją i zostały pominięte:\n\n";
};

struct LabelsLocal_JA {
    const std::u16string analizing_label = u"分析中...";
    const std::u16string start_instructions = u"ファイル名を入力してください:";
    const std::u16string found_list_label = u"一致するファイル";
    const std::u16string no_found_list_label = u"一致するファイルが見つかりませんでした";
    const std::u16string w_max_path_length = u"警告! パスの最大長を超えました。次のパスは使用されませんでした:\n\n";
    const std::u16string w_skipped_paths = u"警告! 次のパスは存在せず、スキップされました:\n\n";
};

struct LabelsLocal_ZH {
    const std::u16string analizing_label = u"正在分析...";
    const std::u16string start_instructions = u"请输入文件名：";
    const std::u16string found_list_label = u"匹配的文件";
    const std::u16string no_found_list_label = u"未找到匹配的文件";
    const std::u16string w_max_path_length = u"警告！超过了路径的最大长度。以下路径未被使用：\n\n";
    const std::u16string w_skipped_paths = u"警告！以下路径不存在，已被跳过：\n\n";
};

struct LabelsLocal_KO {
    const std::u16string analizing_label = u"분석 중...";
    const std::u16string start_instructions = u"파일 이름을 입력하세요:";
    const std::u16string found_list_label = u"일치하는 파일";
    const std::u16string no_found_list_label = u"일치하는 파일을 찾을 수 없습니다";
    const std::u16string w_max_path_length = u"경고! 최대 경로 길이를 초과했습니다. 다음 경로는 사용되지 않았습니다:\n\n";
    const std::u16string w_skipped_paths = u"경고! 다음 경로는 존재하지 않으며 건너뛰었습니다:\n\n";
};