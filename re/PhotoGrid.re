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
  canFetch: bool,
  isModalVisible: bool,
  activePhotoUrl: string
};
type action =
  | SetFetch(bool)
  | ShowPhoto(string)
  | DismissModal
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
  let renderPhoto = (onPressPhoto, photo: photo) => {
    let url = photo.urls.regular;
    <TouchableOpacity onPress=onPressPhoto(url)>
      <View>
        <Image
          style=styles##photo
          resizeMode=`cover
          source=URI(Image.(imageURISource(~uri=url, ())))
        />
      </View>
    </TouchableOpacity>;
  };
  let renderPhotos = (onPressPhoto, photos) =>
    switch (List.length(photos)) {
    | 0 =>
      [|<Text value="No photos..." />|]
    | _ => Array.of_list(List.map(renderPhoto(onPressPhoto), photos))
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
      canFetch: true,
      isModalVisible: false,
      activePhotoUrl: ""
    },
    retainedProps: {photos, fetchPhotos},
    willReceiveProps: (self) => {
      switch (List.length(photos) > List.length(self.retainedProps.photos)) {
      | true => {...self.state, canFetch: true}
      | false => self.state
      };
    },
    reducer: (action, state) => {
      switch action {
      | SetFetch(canFetch) =>
        ReasonReact.Update({...state, canFetch: canFetch});
      | ShowPhoto(url) =>
        ReasonReact.Update({
          ...state,
          isModalVisible: true,
          activePhotoUrl: url
        })
      | DismissModal =>
        ReasonReact.Update({
          ...state,
          isModalVisible: false,
          activePhotoUrl: ""
        })
      | _ => ReasonReact.NoUpdate;
      };
    },
    render: ({reduce, state: {canFetch, isModalVisible, activePhotoUrl}}) => {
      <ScrollView onScroll=reduce(onScroll(canFetch)) scrollEventThrottle=16>
        <PhotoModal isVisible=isModalVisible onDismiss=reduce((_) => DismissModal) photoUrl=activePhotoUrl/>
        <View style=styles##container>
          ...(renderPhotos((url) => reduce((_) => ShowPhoto(url)), photos))
        </View>
      </ScrollView>
    }
  };
};
