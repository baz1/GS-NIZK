(* Proof that (a <= 0) -> (a = 0) *)
Theorem leq_zero :
  forall (a:nat),
  ((a <= 0) -> (a = 0))
.
Proof.
  intro a.
  destruct a.
    (* Case a=0 *)
    intro useless.
    exact (eq_refl 0).
    (* Case a>0 *)
    intro H.
    inversion H.
Qed.

(* Proof that <= is transitive *)
Theorem leq_transitive :
  forall (a:nat) (b:nat) (c:nat), (
    (a <= b) /\ (b <= c) -> (a <= c)
  )
.
Proof.
  intros a b c.
  intro H.
  case H.
  intro H1.
  elim c.
    (* Case c=0 *)
    intro H2.
    rewrite (leq_zero b H2) in H1.
    exact H1.
    (* Case c --> c+1 *)
    intro n.
    intro Hind.
    intro H2.
    inversion H2.
    rewrite H0 in H1.
    exact H1.
    exact (le_S a n (Hind H3)).
Qed.

(* Proof that <= is a totally ordered relation *)
Theorem leq_tot_ordered : forall (a b:nat), (a<=b) \/ (b<=a).
Proof.
  admit.
Qed.

Definition is_modulo (a n r:nat) : Prop :=
  (a<n) /\ (exists (k:nat), r=(k*n+a))
.

Theorem modulo_existence :
  forall (a n:nat), (1<=n) ->
  (exists r, is_modulo a n r).
Proof.
  intros a n H.
  elim a.
    (* Case a=0 *)
    refine (ex_intro _ 0 _).
    unfold is_modulo.
    assert (subg : 0 = 0 * n + 0).
    simpl.
    exact (eq_refl O).
    exact (conj H (ex_intro (fun (k:nat) => 0 = k * n + 0) 0 subg)).
    (* Case a --> a+1 *)
    unfold is_modulo.
    intros n0 H1.
    case H1.
    intros x H2.
    case H2.
    intros H21 H22.
    case H22.
    intros x0 H221.
    pose (test := ((S (S n0)) <= n)).
    (* TODO finish *)
Qed.
