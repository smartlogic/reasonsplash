open BsReactNative;

type urls = {
	full: string,
	regular: string,
	small: string,
	thumb: string
};
type photo = {
	id: string,
	description: option(string),
	likes: int,
	urls: urls
};
type props = {
  photos: list(photo),
  fetchPhotos: string => unit,
  refreshing: bool
};

let windowHeight = Dimensions.get(`window)##height;
let windowWidth = Dimensions.get(`window)##width;
let thumbWidth = float_of_int(windowWidth / 2);
let styles = Style.({
	"container": style([
		flex(1.),
    flexDirection(Row),
    flexWrap(Wrap),
    justifyContent(FlexStart),
    alignItems(Center),
    margin(Pt(0.))
  ]),
	"photo": style([
    width(Pt(thumbWidth)),
    height(Pt(thumbWidth)),
  ])
});

let component = ReasonReact.statelessComponent("PhotoGrid");
let make = (~photos, ~fetchPhotos, ~refreshing, _children) => {
	let renderPhoto = (photo: photo) =>
    <View>
      <Image
        style=styles##photo
        resizeMode=`cover
        source=(
          URI(
            Image.(
              imageURISource(
                ~uri=photo.urls.regular,
                ()
              )
            )
          )
        )
      />
    </View>;
	let renderPhotos = photos =>
		switch (List.length(photos)) {
		| 0 =>
      [|<Text value="No photos..." />|]
		| _ => Array.of_list(List.map(renderPhoto, photos))
		};
  let onScroll = (event: RNEvent.NativeScrollEvent.t) => {
    let contentHeight = RNEvent.NativeScrollEvent.contentSize(event).height;
    let contentOffsetY = RNEvent.NativeScrollEvent.contentOffset(event).y;
    let isEndReached = contentOffsetY +. float_of_int(windowHeight) >= contentHeight +. 10.;

    if (isEndReached && !refreshing)
      fetchPhotos();
  };

	{
		...component,
		render: (_self) =>
      <ScrollView onScroll=onScroll scrollEventThrottle=16 overScrollMode=`never bounces=false>
        <View style=styles##container>
          ...(renderPhotos(photos))
        </View>
      </ScrollView>
	};
};
