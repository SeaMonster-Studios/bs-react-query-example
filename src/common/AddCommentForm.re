module Style = {
  open Css;

  let commentBtnContainer =
    [textAlign(`right), marginBottom(15->px)]->style;

  let formContainer = [display(`flex), flexDirection(`column)]->style;

  let input = [flexGrow(2.)]->style;
};

[@react.component]
let make = (~postId, ~authedUser: Users.user) => {
  let (formIsActive, setFormIsActive) = React.useState(() => false);
  let (comment, setComment) = React.useState(() => "");
  let (createComment, createCommentQ) = Comments.Queries.Create.use();

  React.useEffect2(
    () => {
      switch (createCommentQ.status) {
      | Success(Ok(_)) => setFormIsActive(_ => false)
      | _ => ()
      };
      None;
    },
    (createCommentQ.status, setFormIsActive),
  );

  <>
    <div className=Style.commentBtnContainer>
      {formIsActive
         ? <MaterialUi.Button
             disabled={createCommentQ.status == Loading}
             color=`Secondary
             onClick={_ => setFormIsActive(_ => false)}>
             "Cancel"->React.string
           </MaterialUi.Button>
         : <MaterialUi.Button
             color=`Primary onClick={_ => setFormIsActive(_ => true)}>
             "Add Comment"->React.string
           </MaterialUi.Button>}
    </div>
    {formIsActive
       ? <div className=Style.formContainer>
           <MaterialUi.TextField
             className=Style.input
             value={comment->MaterialUi.TextField.Value.string}
             onChange={event => {
               event->ReactEvent.Form.persist;
               setComment(_ =>
                 (ReactEvent.Form.target(event)##value->Obj.magic: string)
               );
             }}
           />
           <MaterialUi.Button
             color=`Primary
             onClick={_ =>
               createComment(
                 Comments.{
                   postId,
                   body: comment,
                   userId: authedUser.id,
                   id: "",
                   createdAt: "",
                   user: None,
                 },
               )
               ->ignore
             }>
             "Submit"->React.string
           </MaterialUi.Button>
           {switch (createCommentQ.status) {
            | Loading => <Loader />
            | Idle => React.null
            | Error(err) =>
              err->Js.log;
              <ErrorMessage />;
            | Success(Error(err)) =>
              err->Js.log;
              <ErrorMessage />;
            | Success(Ok(_)) => React.null
            }}
         </div>
       : React.null}
  </>;
};