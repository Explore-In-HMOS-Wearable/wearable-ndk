import { router } from '@kit.ArkUI';

@Builder
export function LandingPageBuilder(name: string, param: Object) {
  LandingPage()
}

@Entry
@Component
export struct LandingPage {
  pageInfos: NavPathStack = new NavPathStack();

  build() {
    NavDestination() {
      Scroll() {
        Column({ space: 4 }) {
          Column() {
            Text('ASMR Simulations')
              .fontSize(17)
              .fontColor(Color.White)
              .fontWeight(FontWeight.Bold)
              .onClick(() => {
                router.back()
              })

          }
          .width('100%')
          .padding({ top: 12 })
          .margin({ bottom: 25 })
          .align(Alignment.Top)

          Blank().height(5)


          this.buildMenuItem('Flex Liquid Ball ', Color.Blue, Color.White, Color.Blue,
            () => {
              router.replaceUrl({
                url: 'pages/Index',
              });
            })

          Blank().height(2)

          Text('Interactive Fluid Simulation')
            .fontSize(12)
            .fontColor('#a0a0b0')

          Blank().height(18)

          Blank().height(25)

          Text('Powered By OpenGL ES')
            .fontSize(9)
            .fontWeight(700)
            .fontColor(Color.Red)
        }
        .padding(12)
        .alignItems(HorizontalAlign.Center)
      }
      .width('100%')
      .height('100%')
      .backgroundColor(Color.Black)
    }
    .hideTitleBar(true)
    .onReady((context: NavDestinationContext) => {
      this.pageInfos = context.pathStack
      // Logger.info('current page config info is ' + JSON.stringify(context.getConfigInRouteMap()));
    })
  }

  @Builder
  buildMenuItem(label: string, iconColor: Color, textColor: Color, bgColor: Color,
    action: () => void) {
    Row({ space: 10 }) {

      Text('🌊 '+label)
        .fontSize(15)
        .fontWeight(FontWeight.Bold)
        .fontColor(Color.White)
        .margin({ bottom: 5 })

    }
    .padding({
      left: 8,
      right: 8,
      top: 6,
      bottom: 6
    })
    .backgroundColor(bgColor)
    .borderRadius(20)
    .width('80%')
    .height('45vp')
    .onClick(action)
  }
}
