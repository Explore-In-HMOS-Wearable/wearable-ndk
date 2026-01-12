import nativeRender from 'libentry.so';
import { ArcButtonComponent } from '../components/ArcButtonComponent';
import { ArcButtonPosition } from '@kit.ArkUI';

@Builder
export function IndexBuilder(name: string, param: Object) {
  Index()
}

@Component
export struct Index {
  private xComponentContext: ESObject | undefined = undefined;
  @State metaballCount: number = 0;
  @State isRendering: boolean = false;
  pageInfos: NavPathStack = new NavPathStack();

  private getRandomInt(max: number): number {
    return Math.floor((Date.now() % 10000) / 10000 * max);
  }

  build() {

    NavDestination() {
      Column() {
        ArcButtonComponent({
          label: 'Clear',
          arcPosition: ArcButtonPosition.TOP_EDGE,
          fontSize: 8,
          shadowEnabled: true,
          onButtonClick: () => {
            if (this.xComponentContext) {
              nativeRender.clearMetaballs(this.xComponentContext);
              this.metaballCount = 0;
            }
          }
        })
          .height('5%')
          .zIndex(1)
          .margin({ top: '0%' })


        XComponent({
          id: 'xcomponentId',
          type: XComponentType.SURFACE,
          libraryname: 'entry'
        })
          .onLoad((xComponentContext) => {
            this.xComponentContext = xComponentContext;
            this.isRendering = true;
          })
          .width('100%')
          .height('63%')
          .borderRadius(16)
          .clip(true)
          .margin({ top: '13%' })
          .shadow({
            radius: 20,
            color: 'rgba(0,0,0,0.3)',
            offsetX: 0,
            offsetY: 10
          })


        ArcButtonComponent({
          label: 'Add Random',
          arcPosition: ArcButtonPosition.BOTTOM_EDGE,
          fontSize: 8,
          shadowEnabled: true,
          onButtonClick: () => {

            if (this.xComponentContext) {
              const randomX = this.getRandomInt(26540) % 345;
              const randomY = this.getRandomInt(62540) % 245;
              nativeRender.addMetaball(this.xComponentContext, randomX, randomY);
              this.metaballCount++;
            }
          }
        })
          .height('7%')
          .margin({ bottom: '0%' })

      }
      .width('100%')
      .height('100%')

    }
    .hideTitleBar(true)
    .onReady((context: NavDestinationContext) => {
      this.pageInfos = context.pathStack

    })

  }
}