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
  fetchPhotos: unit => unit
};
type state = {
  canFetch: bool
};
type action =
  | SetFetch(bool)
  | NoUpdate;

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

let component = ReasonReact.reducerComponentWithRetainedProps("PhotoGrid");
let make = (~photos, ~fetchPhotos, _children) => {
  let renderPhoto = (photo: photo) =>
    <View>
      <Image
        style=styles##photo
        resizeMode=`cover
        source=URI(Image.(imageURISource(~uri=photo.urls.regular, ())))
      />
    </View>;
  let renderPhotos = photos =>
    switch (List.length(photos)) {
    | 0 =>
      [|<Text value="No photos..." />|]
    | _ => Array.of_list(List.map(renderPhoto, photos))
    };
  let onScroll = canFetch => (event: RNEvent.NativeScrollEvent.t) => {
    let contentHeight = RNEvent.NativeScrollEvent.contentSize(event).height;
    let contentOffsetY = RNEvent.NativeScrollEvent.contentOffset(event).y;
    let isEndReached = contentOffsetY +. float_of_int(windowHeight) >= contentHeight +. 10.;

    if (isEndReached && canFetch) {
      fetchPhotos();
      SetFetch(false);
    } else {
      NoUpdate;
    };
  };

  {
    ...component,
    initialState: () => {
      canFetch: true
    },
    retainedProps: {photos, fetchPhotos},
    willReceiveProps: (self) => {
      switch (List.length(photos) > List.length(self.retainedProps.photos)) {
      | true => {canFetch: true}
      | false => self.state
      };
    },
    reducer: (action, _state) => {
      switch action {
      | SetFetch(canFetch) =>
        ReasonReact.Update({canFetch: canFetch});
      | _ => ReasonReact.NoUpdate;
      };
    },
    render: (self) =>
      <ScrollView onScroll=self.reduce(onScroll(self.state.canFetch)) scrollEventThrottle=16>
        <View style=styles##container>
          ...(renderPhotos(photos))
        </View>
      </ScrollView>
  };
};
