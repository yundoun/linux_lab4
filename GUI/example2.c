#include <gtk/gtk.h>

// 버튼이 클릭될 때 호출되는 콜백 함수
void on_button_clicked(GtkWidget *button, gpointer data)
{
  GtkWidget *label = GTK_WIDGET(data);

  // 레이블의 텍스트를 변경하여 "Button Clicked!"으로 설정
  gtk_label_set_text(GTK_LABEL(label), "버튼 클릭 !");
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv); // GTK 초기화

  // 최상위 윈도우 생성
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GTK 예제 2");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_widget_set_size_request(window, 300, 200);

  // 윈도우가 닫힐 때 프로그램 종료
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // 초기 레이블 텍스트를 "Hello, GTK!"으로 설정
  GtkWidget *label = gtk_label_new("Hello, GTK!");

  // "Click Me" 레이블을 가진 버튼 생성
  GtkWidget *button = gtk_button_new_with_label("버튼을 클릭하세요");

  // 버튼 클릭 시 호출될 콜백 함수를 연결
  g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), label);

  // 수직 박스 컨테이너 생성하고 레이블과 버튼을 추가
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

  // 윈도우에 박스를 추가
  gtk_container_add(GTK_CONTAINER(window), box);

  // 윈도우와 그 내부 위젯들을 모두 표시
  gtk_widget_show_all(window);

  // GTK 이벤트 루프 실행
  gtk_main();

  return 0;
}
