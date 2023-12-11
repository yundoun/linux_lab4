#include <gtk/gtk.h>

int count = 0; // 카운트 변수를 선언하고 초기값을 0으로 설정

// "Increment" 버튼이 클릭되면 호출되는 콜백 함수
void on_button_clicked(GtkWidget *widget, gpointer data)
{
  count++; // 카운트 변수를 증가시킴

  // 카운트 값을 문자열로 변환
  gchar *count_text = g_strdup_printf("Count: %d", count);

  // 레이블 위젯에 카운트 값을 표시
  gtk_label_set_text(GTK_LABEL(data), count_text);

  // 메모리 해제
  g_free(count_text);
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv); // GTK 초기화

  // 최상위 윈도우 생성
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GTK 예제 3");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_widget_set_size_request(window, 300, 200);

  // 윈도우를 닫을 때 프로그램 종료
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // 수직 박스 컨테이너 생성
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), box);

  // 초기 카운트 값을 표시하는 레이블 생성
  GtkWidget *label = gtk_label_new("Count: 0");
  gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

  // "Increment" 버튼 생성
  GtkWidget *button = gtk_button_new_with_label("카운터 증가");
  gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

  // "Increment" 버튼 클릭 시 호출될 콜백 함수 연결
  g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), label);

  // 윈도우와 위젯들을 표시
  gtk_widget_show_all(window);

  // GTK 이벤트 루프 실행
  gtk_main();

  return 0;
}
