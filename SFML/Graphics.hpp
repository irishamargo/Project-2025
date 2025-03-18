
using namespace sf;

int main(){
	RenderWindow window(VideoMode(400, 500), "Hello!");
	while(window.isOpen()){
		//класс события
		Event event;
		//обработка очереди событий
 		while(window.pollEvent(event)){
 			//проверка на событие "закрытия окна" - нажатие крестика
			if(event.type == Event::Closed){
				//закрываем окно
				window.close();
			}
	    }
    }
	window.clear(Color(255, 255, 255, 0));
	window.display();
		return 0;
}
