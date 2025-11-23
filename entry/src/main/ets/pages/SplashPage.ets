import { curves } from '@kit.ArkUI';
import { AnimatedText } from '../components/AnimatedText';

@Builder
export function SplashPageBuilder(name: string, param: Object) {
  SplashPage()
}

@Component
export struct SplashPage {
  private timerId = 0;
  @State planeOffsetX: number = -400;
  @State planeOffsetY: number = 350;
  @State animateEarth: boolean = false;
  @State earthTranslateY: number = 300;
  pageInfos: NavPathStack = new NavPathStack();

  aboutToAppear() {

    setTimeout(() => {
      this.startPlaneAnimation();
    }, 100);

    this.timerId = setTimeout(() => {

      this.pageInfos.pushPathByName('MenuPage', null);

    }, 2000);
  }

  startPlaneAnimation() {
    animateTo({
      curve: curves.initCurve(),
      duration: 4500,
      delay: 0,
      iterations: 1,
      playMode: PlayMode.Normal
    }, () => {
      this.planeOffsetX = 500;
      this.planeOffsetY = -150;
    });
  }

  onDestroy() {
    if (this.timerId) {
      clearTimeout(this.timerId);
    }
  }

  build() {

    NavDestination() {

      Stack() {

        AnimatedText()
          .position({
            y: 60
          })

      }
      .width('100%')
      .height('100%')
      .backgroundColor(Color.Black)

    }
    .hideTitleBar(true)
    .onReady((context: NavDestinationContext) => {
      this.pageInfos = context.pathStack
    })
  }
}