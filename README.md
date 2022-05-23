# SnakeGame_21020550_NgoHanQuangNgoc
Final Project INT2215 1

IDE sử dụng: XCode 13.3.

Thiết bị sử dụng: Macbook pro M1 pro 2021.

I. Cài đặt chương trình.
1. Tải Xcode 13.3, tải GithubDesktop và chỉnh ứng dụng mặc định thành Xcode. 
2. Trong GithubDesktop, clone url: https://github.com/quangngoc21020550/SnakeGame_21020550_NgoHanQuangNgoc.git và click vào Open in Xcode. 
3. Tải các file ảnh và font chữ cần thiết của chương trình: https://drive.google.com/drive/folders/1Ed_ga53PVShLRDV1AEmmLXb6Us7eFB0h?usp=sharing.
4. Cài đặt SDL tại https://www.libsdl.org/download-2.0.php#source, SDL_ttf tại https://www.libsdl.org/projects/SDL_ttf/release/ (tải file có tên "SDL2_ttf-2.0.18.dmg";và add vào frameworks của máy như hướng dẫn trong link https://lazyfoo.net/tutorials/SDL/01_hello_SDL/mac/index.php (làm y hệt tương tự với SDL_ttf).
5. Link SDL với code: https://youtu.be/gDTlVV9hgtk.
6. Bấm Run hoặc Cmd+R để chạy chương trình (chương trình hiện Run Succeed nghĩa là đã thành công).
7. Lưu ý quan trọng khi chạy chương trình: https://youtu.be/Ovy8_nQbE-w. (Do kênh Youtube kia bị quá giới hạn upload video nên em dùng kênh Youtube khác ạ).

II. Mô tả chung về trò chơi.
Trò chơi tên Snake Game, được lấy cảm hứng chính từ trò chơi con rắn cổ điển, xây dựng theo phong cách đồ họa pixel và bổ sung thêm một số tính năng: Tường chặn, chọn chế độ tường chặn, ăn lá chắn giúp bất tử một thời gian, tính điểm, tăng tốc độ khi điểm tăng.

III. Mô tả chi tiết các tính năng. (Em mô tả bằng lời chi tiết trong video youtube)
1. Những tính năng cơ bản của trò chơi con rắn cổ điển(tăng độ dài rắn khi ăn táo, khi đi ra khỏi màn hình thì xuất hiện ở đầu bên kia, thua khi đầu chạm vào đuôi).
Link Youtube: https://youtu.be/SlLQAjW8UBM.
2. Tính năng shield.
Link Youtube: https://youtu.be/2esUaPsXYdY.
3. Tính năng menu và chọn chế độ chơi.
Link Youtube p1: https://youtu.be/iqaWeQUpek0.
Link Youtube p2: https://youtu.be/qPwnc0NAMFM.
4. Tính năng tường.
Link Youtube: https://youtu.be/VuVIF7Zuxyw.
5. Tính năng tính điểm.
Link Youtube:https://youtu.be/wFqCsqqwGig.

IV. Các kỹ thuật được sử dụng trong lập trình.
1. Mảng: Chương trình Snake game đã sử dụng các mảng động: deque và vector. Và không sử dụng mảng tĩnh array. Deque trong chương trình có tác dụng thêm liên tục các phần tử snake, để hiển thị rắn ra khỏi màn hình cũng như kiểm tra khi nào rắn tự đâm đầu vào đuôi của mình. Lý do dùng deque thay vì vector là vì deque co thể thêm phần tử vào vị trí đầu tiên thay vì chỉ cuối cùng như vector. Vector trong chương trình được sử dụng để chứa các phần tử tường, nhằm kiểm tra khi nào rắn đâm vào tường.
2. Cấu trúc (Struct): Struct đóng vai trò quan trọng trong việc nâng cấp chương trình trong tương lai, khi sử dụng struct hay class, ngoài tác dụng khiến code gọn, dễ hiểu hơn, thì ta có thể tạo ra các chế độ nhiều người chơi, tạo ra nhiều loại con mồi (trong game đã có táo và lá chắn).
3. Con trỏ: Con trỏ được sử dụng đa phần trong hai hàm khỏi tạo cửa sổ: initSDL và quitSDL. Đa phần các hàm và phương thức trong bài sử dụng biến tham số. Em lựa chọn biến tham số vì nó có thể giúp chương trình tiết kiệm bộ nhớ và dễ sử dụng hơn nhiều so với biến con trỏ.
4. Đồ họa: Đồ họa của game theo phong cách pixel tối giản. Nên không có những thuật toán nâng cao của đồ họa. Những gì em sử dụng trong bài đơn giản là tô màu, vẽ các phần tử SDL_Rect và hiển thị ảnh bitmap.

V. Kết luận, hướng phát triển và các điều tâm đắc rút ra được.
1. Kết luận.
* Dự án Snake Game của em đã thành công mô phỏng lại được tựa game rắn cổ điển, đồng thời cũng bổ sung thêm nhiều tính năng. Code của chương trình tuy có những đoạn chưa được tối ưu nhưng nhìn chung em vẫn cảm thấy sạch đẹp, dễ hiểu cho người đọc, tên biến, tên hàm cụ thể và đúng chức năng. Những tính năng của game vẫn còn khá đơn giản, tuy nhiên dễ dàng phát triển trong tương lai nhờ vào việc sử dụng các cấu trúc (Struct). Những điểm em chưa hài lòng với dự án của mình là chương trình vẫn chưa có chế độ nhiều người chơi như những gì em đã đặt ra ban đầu, đồng thời cũng không có âm thanh do thư viện SDL_mixer không chạy được trên máy tính, điều này khiến trải nghiệm chơi game chưa được trọn vẹn.
2. Hướng phát triển.
* Do đã tận dụng tốt các Struct nên việc phát triển game trong tương lai trở nên vô cùng triển vọng. Mục tiêu đầu tiên của em là chế độ hai người đấu với nhau và dần mở rộng ra thành bốn người, việc này cần học cách giúp chương trình nhận nhiều tín hiệu đầu vào cùng một lúc, điều mà em chưa làm được trong dự án lần này. Hướng phát triển tiếp theo là tạo ra nhiều lại con mồi vói tính năng thú vị, ví dụ như giảm độ dài rắn tạm thời hoặc tăng tốc độ cho rắn. Tiếp đến là đa dạng hóa các loại tường, không đơn giản chỉ là đâm vào rồi thua như trong dự án. Nhìn chung, việc phát triển nhiều tính năng thú vị của game rất khả thi và triển vọng.
3. Những điều rút ra được.
* Thực hiện dự án đầu tiên giúp em trải nghiệm được cảm giác hạnh phúc khi cho ra một sản phẩm thực sự có vai trò trong cuộc sống. Trước kia khi học lập trình, em và các sinh viên khác đa phần giải những bài toán liên quan đến xử lý các số liệu. Chính vì thế, khi làm chương trình Snake Game giúp em cảm thấy phấn khích, thú vị hơn vì mình được lập trình chính chủ đề mình đã chọn, đồng thời cũng càng cảm thấy đúng đắn khi đã lựa chọn ngành Công nghệ thông tin.
* Lần đầu code một chương trình game cũng khiến em hiểu được khó khăn của những lập trình viên khác, vì việc làm một trò chơi đồ họa đơn giản 2d với các chức năng cơ bản đã tốn của em nhiều trí lực. Vậy nên để tạo ra những tựa game bom tấn hay những chương trình quy mô lớn phải rất khó khăn cho các lập trình viên.
* Điều đặc biệt nhất là dự án lần này đã giúp em làm quen dần với việc lập trình một cách chuyên nghiệp, thay vì code toàn bộ trong main, đặt tên biến theo bảng chữ cái như lúc trước, hiện giờ em đã tách lớp, tách hàm, đặt tên biến, xử lý các logic sao cho chương trình sạch đẹp, dễ hiểu nhất có thể.

