module Style = {
  open Css;

  let root =
    [display(`flex), alignItems(`center), justifyContent(`center)]->style;
};

[@react.component]
let make = () => {
  <div className=Style.root> <MaterialUi.CircularProgress /> </div>;
};