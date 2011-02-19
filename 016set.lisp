;; Extensible assignment

(defover = setf)

(defmacro defset(type args &rest body)
  `(defcoerce ,type function=
    (lambda ,args
      ,@body)))



;; Internals

(defun call-setf(&rest args)
  (indexing wart-coercions* ('function=
                             (wart-type (car args)))
    (apply it args)))
(defsetf call-fn call-setf)
