module Style = {
  open Css;

  let sectionPadding = padding2(~v=10->px, ~h=15->px);
  let sectionBorder = borderBottom(1->px, `solid, `hex("eee"));

  let author = [sectionPadding, display(`flex), alignItems(`center)]->style;

  let authorName = [marginLeft(15->px)]->style;

  let body = [sectionPadding]->style;

  let footer = [sectionPadding]->style;
};

[@react.component]
let make = (~author: Users.user, ~children: React.element, ~footer=React.null) => {
  <MaterialUi.Card>
    <header className=Style.author>
      <MaterialUi.Avatar src={author.avatar} />
      <div className=Style.authorName> author.name->React.string </div>
    </header>
    <div className=Style.body> children </div>
    {footer == React.null
       ? footer : <footer className=Style.footer> footer </footer>}
  </MaterialUi.Card>;
};