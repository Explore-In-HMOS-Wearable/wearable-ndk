import { curves } from '@kit.ArkUI';
import { AnimatedText } from '../components/AnimatedText';

@Builder
export function SplashPageBuilder(name: string, param: Object) {
  SplashPage()
}

@Entry
@Component
export struct SplashPage {
  private timerId = 0;
  planeOffsetX: number = -400;
  planeOffsetY: number = 350;
  animateEarth: boolean = false;
  earthTranslateY: number = 300;
  pageInfos: NavPathStack = new NavPathStack();

  aboutToAppear() {
    this.timerId = setTimeout(() => {
      this.pageInfos.pushPathByName('MenuPage', null);
    }, 2000);
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
          .transition(
            TransitionEffect.OPACITY
              .animation({ duration: 300, curve: Curve.EaseInOut })
              .combine(TransitionEffect.scale({ x: 0.8, y: 0.8 }))
          )
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